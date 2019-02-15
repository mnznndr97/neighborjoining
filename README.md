# Neighbor joining
C implementation of the Neighbor Joining algorithm with distances read from a FASTA file

## Repo dependencies
[Levenshtein distance](https://github.com/wooorm/levenshtein.c/ "Levenshtein distance submodule") for the edit distance calculation

## Building 
The project depends on CMake for the compilation
It may require `gengetopt` in order to recreate the parser code for the command line

```
$ cmake ./
$ make
```


## Usage
This is the command to invoke `neighborjoining-linux`.
It outputs the phylogenetic tree associated to an input FASTA file

```
$ neighborjoining-linux path/to/input/file.fa
```
## FASTA parsing limitations 

Since FASTA can presents itself in lots of different formats, for the moment we can parse only a file that follows the specified rules:

- Each sequence must start with the `>` character
   ```
   >[seq ... other text] 
   ```
- Each sequence must be separated by an empty line
    ```
    >seq0
    FQTWEEFSRAAEKLYLADPM

    >seq1
    KYRTWEEFTRAAEKLYQA
    ```
## Output example 
Command:
```
$ neighborjoining-linux examples/fasta_simple.fa
```

Output:
```
Phylogenetic tree:
                    +--5.00-->[C0]
          +--2.00-->[J1]
          |         +--46.00-->[C1]
+--4.75-->[J2]
|         +--6.50-->[C2]
[J3]
+--4.75-->[C3]
```
## Links
[Academic paper on neighbor joining](https://academic.oup.com/mbe/article/23/11/1997/1322446)

[Wikipedia](https://en.wikipedia.org/wiki/Neighbor_joining)
