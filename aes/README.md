# Usage
```
./cipher FILENAME -m MODE -k KEY -i IV [-vgted]

  -v, --version       for software version
  -m, --mode=[value]  for mode choice. (ecb/cbc)
  -e, --enc           flag for encryption mode (conflicts with -d)
  -d, --dec           flag for decryption mode (conflicts with -e)
  -k, --key=[value]   for key init
  -i, --iv=[value]    for initialization vector
  -g, --debug         for debug values
  -t, --time          for timing
```
