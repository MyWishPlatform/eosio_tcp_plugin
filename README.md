Clone repo:
```bash
eos/plugins]$ git clone https://github.com/MyWishPlatform/eosio_tcp_plugin/
eos/plugins]$ mv eosio_tcp_plugin tcp_plugin
```

<br />

Modify eos/plugins/CMakeLists.txt:
```
...
add_subdirectory(tcp_plugin)
...
```

<br />

Modify eos/programs/nodeos/CMakeLists.txt:
```
...
PRIVATE -Wl,${whole_archive_flag} tcp_plugin  -Wl,${no_whole_archive_flag}
...
```

<br />

Compile:
```bash
eos/build]$ make
eos/build]$ sudo make install
```

<br />

Add to config.ini:
```
tcp-plugin-port = 56731 # If you need other port than default 56732
...
plugin = eosio::tcp_plugin # Should be placed before plugins that using it
...
```
