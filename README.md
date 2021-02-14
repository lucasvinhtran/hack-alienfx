# hack-alienfx

Follow the steps below:

1. 
```
cd hack-alienfx
```

2.
```
lsusb
grep -rin "0530" ./
```
replace 0530 with the id


3.
```
make all
sudo ./run seq/snooze
sudo ./run seq/afx-godark
sudo ./run seq/afx-off
sudo ./run seq/afx-allblue
```

