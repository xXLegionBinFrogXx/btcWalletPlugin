#include "bitcoinwalletplugin.h"
#include <QDebug>

extern "C" {
#include "secp256k1_wrapper.h"
}

#include "bitcoin_key_utils.h"

BitcoinWalletPlugin::BitcoinWalletPlugin(QObject *parent)
    : QObject(parent)
{}

BitcoinWalletPlugin::~BitcoinWalletPlugin() = default;

std::expected<IWalletInterface::Wallet, QString> BitcoinWalletPlugin::generateWallet(const QString& vanityPrefix) const noexcept
{
    try {


        unsigned char privkey_buf[PRIVKEY_SIZE];
        unsigned char pubkey_buf[PUBKEY_COMPRESSION_SIZE];

        int res = secp256k1_wrapper_generate_keys(privkey_buf, pubkey_buf,  1 );
        if (res != 0) {
            return std::unexpected(QStringLiteral("secp256k1-wrapper: failed to generate keypair, error code: %1").arg(res));
        }

        unsigned char derived_pubkey_buf[PUBKEY_COMPRESSION_SIZE];
        res = secp256k1_wrapper_derive_pubkey(privkey_buf, derived_pubkey_buf, 1);
        if (res != 0) {
            return std::unexpected(QStringLiteral("Failed to derive public key for verification, error code: %1").arg(res));
        }

        if (!compareKey(pubkey_buf,derived_pubkey_buf,PUBKEY_COMPRESSION_SIZE)){
            return std::unexpected(QStringLiteral("Mismatched public keys"));
        }


        const std::vector<uint8_t> privateKey(privkey_buf, privkey_buf + PRIVKEY_SIZE);

        auto wifCompressed = BitcoinKeyUtils::EncodeWIF(privateKey, true);
        if (!wifCompressed) {
            return std::unexpected(
                QStringLiteral("WIF encoding failed: %1").arg(QString::fromStdString(wifCompressed.error().message))
                );
         }

        const std::vector<uint8_t> pubKey(pubkey_buf,pubkey_buf+PUBKEY_COMPRESSION_SIZE);
        auto pubKeyHashExp = BitcoinKeyUtils::HashRIPEMD160SHA256(pubKey);
        if (!pubKeyHashExp) {
            return std::unexpected(
                QStringLiteral("Hash160 failed: %1").arg(QString::fromStdString(pubKeyHashExp.error().message))
                );
        }

        auto p2wpkh = BitcoinKeyUtils::GenerateP2WPKHAddress(pubKeyHashExp.value());
        if (!p2wpkh) {
            return std::unexpected(
                QStringLiteral("P2WPKH encoding failed: %1").arg(QString::fromStdString(p2wpkh.error().message))
                );
        }

        IWalletInterface::Wallet wallet;
        wallet.privateKey = QString::fromStdString(wifCompressed.value());
        wallet.address = QString::fromStdString(p2wpkh.value());
        return wallet;
    } catch (const std::exception& e) {
        return std::unexpected(QString("Failed to generate wallet: %1").arg(e.what()));
    }
}


bool BitcoinWalletPlugin::compareKey(const unsigned char key1[], const unsigned char key2[], size_t keySize)const {
    if (!key1 || !key2 ) {
        return false;
    }
    return std::memcmp(key1, key2, keySize) == 0;
}

bool BitcoinWalletPlugin::isAvailable() const noexcept
{
    return true; 
}

QString BitcoinWalletPlugin::pluginName() const noexcept
{
    return QString("Bitcoin Segwit");
}

QString BitcoinWalletPlugin::derivationPath() const noexcept
{
    return QString("m/84'/0'/0'");
}

QString BitcoinWalletPlugin::pluginVersion() const noexcept
{
    return QString("v2.1.0");
}

QString BitcoinWalletPlugin::pluginDescription() const noexcept
{
    return QString("Provides Bitcoin Segwit wallet generation.");
}

QString BitcoinWalletPlugin::addressSample() const noexcept
{
    return QString("bc1qh7tx2ten7dmyqx5htm5u709dhasvvrmm2nylxr");
}

QByteArray BitcoinWalletPlugin::convertToQByteArray(const unsigned char *data, int size) const
{
    return QByteArray(reinterpret_cast<const char*>(data), size);

}

void BitcoinWalletPlugin::convertFromQByteArray(const QByteArray &data, unsigned char *output) const
{
    std::memcpy(output, data.constData(), static_cast<size_t>(data.size()));
}

