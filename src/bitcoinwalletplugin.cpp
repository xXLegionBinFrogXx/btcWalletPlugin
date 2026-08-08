#include "bitcoinwalletplugin.h"

#include "bitcoin_key_utils.h"

#include <exception>

namespace {

QString toErrorString(const BitcoinKeyUtils::Error &error)
{
    return QString::fromStdString(error.message);
}

// Must only be called from inside a catch block.
QString currentExceptionMessage() noexcept
{
    try {
        throw;
    } catch (const std::exception &e) {
        return QStringLiteral("Failed to generate wallet: %1").arg(QString::fromUtf8(e.what()));
    } catch (...) {
        return QStringLiteral("Failed to generate wallet: unknown exception");
    }
}

}

BitcoinWalletPlugin::BitcoinWalletPlugin(QObject *parent)
    : QObject(parent)
{}

BitcoinWalletPlugin::~BitcoinWalletPlugin() = default;

std::expected<IWalletInterface::Wallet, QString> BitcoinWalletPlugin::generateWallet(const QString &vanityPrefix) const noexcept
{
    try {
        if (!vanityPrefix.isEmpty()) {
            return std::unexpected(QStringLiteral("Vanity address generation is not supported"));
        }

        auto generated = BitcoinKeyUtils::Wallet::generate();
        if (!generated) {
            return std::unexpected(
                QStringLiteral("Key generation failed: %1").arg(toErrorString(generated.error()))
                );
        }

        return Wallet{
            .privateKey = QString::fromStdString(generated->wif()),
            .address = QString::fromStdString(generated->p2wpkhAddress()),
        };
    } catch (...) {
        return std::unexpected(currentExceptionMessage());
    }
}

bool BitcoinWalletPlugin::isAvailable() const noexcept
{
    return true;
}

QString BitcoinWalletPlugin::pluginName() const noexcept
{
    return QStringLiteral("Bitcoin Segwit");
}

QString BitcoinWalletPlugin::derivationPath() const noexcept
{
    return QString();
}

QString BitcoinWalletPlugin::pluginVersion() const noexcept
{
    return QStringLiteral("v3.0.0");
}

QString BitcoinWalletPlugin::pluginDescription() const noexcept
{
    return QStringLiteral("Provides Bitcoin Segwit wallet generation.");
}

QString BitcoinWalletPlugin::addressSample() const noexcept
{
    return QStringLiteral("bc1qh7tx2ten7dmyqx5htm5u709dhasvvrmm2nylxr");
}
