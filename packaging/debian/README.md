## The Debian Package trisdb

This package contains the TrisDB server, conf file and init script for Ubuntu/Debian.

Packaging:
```
cd trisdb-*
ln -s packaging/debian .
```
Update debian/changelog and rename parent dir to correct version
```
debuild -S -sa
cd ..
dput ppa:trisdb/ppa trisdb_0.0.14_source.changes
```
