# enigma16

enigma16 is a cipher based on enigma machine algorithm. it differs in alphabet size and rotor... rotations. here, the letter is a 4bit data nibble, hence it could be used for anything that can be encoded as bytes.
the "key" is just a nibble pairs, so `0xA` input may be mapped to `0x2` rotor input, and vice versa. key may be generated using `-g`(generate) flag.

## usage example
```sh
$ cat > test << EOF
naked ponies online free download
nyaa dot si
EOF

$ cat test | e16 ce6af528793b0d14 > test.e16

$ cat test.e16 
�4M�%[<J!�:�D�+ʖ7f���Җ�6�1���>��KH�
                                   �

$ cat test.e16 | e16 ce6af528793b0d14
naked ponies online free download
nyaa dot si
```
