# enigma16

enigma16 is a cipher based on enigma machine algorithm. it differs in alphabet size and rotor... rotations. here, the letter is a 4bit data nibble, hence it could be used for anything that can be encoded as bytes.
the "key" is just a nibble pairs, so `0xA` input may be mapped to `0x2` rotor input, and vice versa. key may be generated using `-g`(generate) flag.

please don't take it seriously. enigma was cracked 100 years ago. this project made purely for fun and to make a joke about mlp porn in readme file 

## usage example
```sh
$ cat > test << EOF
naked ponies online free download
nyaa dot si
EOF

# -h flag tells e16 to output data as plaintext hex
$ cat test | e16 ce6af528793b0d14 -h > test.e16

$ cat test.e16 
6775941c7c164f573e03e5e825749a947495a3e119ab80a8fec7389612b922ccd821fd0a4f3126c5dced131dccf4

# -x flags tells e16 that input is plantext hex
$ cat test.e16 | e16 ce6af528793b0d14 -x
naked ponies online free download
nyaa dot si
```

## verifying 
simplest way to verify & rebuild e16 is to encrypt and decrypt e16 source file, and send it to the compiler.
```sh
$ cat e16.c | e16 | e16 | gcc -o e16 -x c -
```
