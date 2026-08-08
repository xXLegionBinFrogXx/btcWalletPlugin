#ifndef BITCOINWALLETPLUGIN_H
#define BITCOINWALLETPLUGIN_H

#include "iwalletinterface.h"
#include <QObject>


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
    QString pluginVersion() const noexcept override;

    [[nodiscard]]
    QString pluginDescription() const noexcept override;

    [[nodiscard]]
    QString addressSample() const noexcept override;
};

#endif // BITCOINWALLETPLUGIN_H
