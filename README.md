MarkdownPanda: WORK IN PROGRESS
-------------------------------

## To compile:

#### First compile MyHTML

- `git submodule update --init`
- `cd external-libraries/myhtml`
- `make`
- `make install PREFIX="../.."`

#### Then compile the project

- `mkdir cmake && cd cmake`
- `cmake ..`
- `make`




