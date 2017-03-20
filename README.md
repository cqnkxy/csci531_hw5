# USC CSCI531 HW5

An assignment about AES-128 and arithematics in GF(<sup>2</sup>).

## Compile

type `make` or `make HW5` to generate executable.
type `make clean` to clear up *.o file and executable.

## Command

### checktable file: hw5 tablecheck -t=tablefile
Check the integrity of tablefile. Please see [tablefile format](http://merlot.usc.edu/cs531-s17/homeworks/hw5/#format) to see the requirements.

### modular product: hw5 modprod -p1=poly1 -p2=poly2
Perform modular product (modulo {01}x<sup>4</sup>+{01}) of two polynomials with coefficients in GF(2<sup>8</sup>). (Please see equation (4.11) and (4.12) on page 13 of [FIPS-197](https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxyZWdodTExY3M5NjQxM3xneDo0ZTMwODdiMWQ5ODg0OTY4) to see how to perform modular product.) The coefficients of the polynomials are represented as hexstrings in `poly1` and `poly2` (each hexstring has a string length of 8). The first two hex digits correspond to the coefficient for x<sup>3</sup>, the next two hex digits correspond to the coefficient for x<sup>2</sup>, etc.

### key sheduling: hw5 keyexpand -k=key -t=tablefile
Perform the key expansion algorithm for AES-128 using key as the 16-byte cipher key in hexstring format (32 characters long). Use the s-box in tablefile.

### AES encrypt: hw5 encrypt -k=key -t=tablefile [file]
Encrypts the first 16 bytes of the input using AES-128 driven by tablefile in the ECB mode using key as the encryption key (16 bytes long and in hexstring format). If the filesize of the input is less than 16 bytes, print an error message. (Treat the input as a binary file.)

The output of this command should look like page 36 of the [FIPS 197 AES specification](https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxyZWdodTExY3M5NjQxM3xneDo0ZTMwODdiMWQ5ODg0OTY4).

### AES decrypt: hw5 decrypt -k=key -t=tablefile [file]
Decrypts the first 16 bytes of the input using AES-128 driven by tablefile in the ECB mode using key as the encryption key (16 bytes long and in hexstring format). If the filesize of the input is less than 16 bytes, print an error message. (Treat the input as a binary file.)

The output of this command should look like page 36 and 37 of the [FIPS 197 AES specification](https://docs.google.com/viewer?a=v&pid=sites&srcid=ZGVmYXVsdGRvbWFpbnxyZWdodTExY3M5NjQxM3xneDo0ZTMwODdiMWQ5ODg0OTY4).

### multiplicative inverse: hw5 inverse -p=poly
Calculate the multiplicative inverse (modulo {01}x<sup>4</sup>+{01}) of the polynomial poly with coefficients in GF(2<sup>8</sup>) using the extended Euclidean algorithm.
