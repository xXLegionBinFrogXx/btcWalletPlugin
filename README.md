# btcWalletInterface

A Qt plugin implementing [`IWalletInterface`](https://github.com/xXLegionBinFrogXx/WalletInterfaces)
for Bitcoin native SegWit (P2WPKH) wallet generation. Builds as a shared library
loaded by the host application through `QPluginLoader`.

- **Interface:** `IWalletInterface` 2.0
- **Key generation:** secp256k1, compressed keys
- **Output:** private key in WIF (compressed), address in P2WPKH (`bc1...`)
- **Requires:** CMake 3.24+, Qt 6 Core, a C++23 compiler, Linux

## Building

All dependencies are fetched at configure time; only Qt 6 must be present.

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=<qt-prefix>
cmake --build build
```

The plugin lands in `build/plugins/libBitcoinWalletPlugin.so`.

Dependencies pulled in by `FetchContent`:

| Dependency | Version |
| --- | --- |
| [WalletInterfaces](https://github.com/xXLegionBinFrogXx/WalletInterfaces) | `v2.0.0` |
| [secp256k1-wrapper](https://github.com/xXLegionBinFrogXx/secp256k1-wrapper) | `v1.1.0` |
| [bitcoin-key-utils](https://github.com/xXLegionBinFrogXx/bitcoin-key-utils) | `v1.1.0` |

`WalletInterfaces` uses `FIND_PACKAGE_ARGS`, so an installed copy matching 2.0 is
preferred and the clone is the fallback.


## Security

Keys are generated in process memory and returned as `QString`. Nothing is
persisted, and no key material is logged. Callers are responsible for the
lifetime of the returned values.

## License

MIT. See `LICENSE`.
