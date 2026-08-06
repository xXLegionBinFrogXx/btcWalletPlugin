#ifndef BITCOINWALLETPLUGIN_H
#define BITCOINWALLETPLUGIN_H

#include "iwalletinterface.h"
#include <QObject>


#define PRIVKEY_SIZE 32
#define PUBKEY_COMPRESSION_SIZE 33

class BitcoinWalletPlugin : public QObject, public IWalletInterface
{
    Q_OBJECT
    Q_INTERFACES(IWalletInterface)
    Q_PLUGIN_METADATA(IID IWalletInterface_IID FILE "../bitcoinwalletplugin.json")

public:
    explicit BitcoinWalletPlugin(QObject *parent = nullptr);
    ~BitcoinWalletPlugin() override;

    [[nodiscard]]
    std::expected<Wallet, QString> generateWallet(const QString &vanityPrefix = QString()) const noexcept override;

    [[nodiscard]]
    bool isAvailable() const noexcept override;

    [[nodiscard]]
    QString derivationPath() const noexcept override;

    [[nodiscard]]
    QString pluginName() const noexcept override;

    [[nodiscard]]
    virtual QString pluginVersion() const noexcept override;

    [[nodiscard]]
    virtual QString pluginDescription() const noexcept override;

    [[nodiscard]]
    virtual QString addressSample() const noexcept override;



private:
    bool compareKey(const unsigned char key1[], const unsigned char key2[], size_t keySize) const;
    QByteArray convertToQByteArray(const unsigned char* data, int size) const;
    void convertFromQByteArray(const QByteArray& data, unsigned char* output) const;

};

#endif // BITCOINWALLETPLUGIN_H
