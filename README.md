MarkdownPanda: WORK IN PROGRESS
-------------------------------

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
**Table of Contents**

- [-](#-)
- [To compile:](#to-compile)
    - [-](#--1)
    - [Then compile the project](#then-compile-the-project)

<!-- markdown-toc end -->


## To compile:

#### First compile MyHTML

- `git submodule update --init`
- `cd external-libraries/myhtml`
- `make`
- `make install prefix="../.."`

#### Then compile the project

- `mkdir cmake && cd cmake`
- `cmake ..`
- `make`




