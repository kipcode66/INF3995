# To Run test cases

## Setup on your local machine
 - Required resources:
    - python3
    - pip
    - venv (included in python3)

 - create a venv in this folder:
    `python3 -m venv venv`

 - activate the venv :
    `source ./venv/bin/activate`

 - install required python libs
    `pip install -r requirements.txt`

 - run all test cases:
    `python3 restapitest.py`

 - be glad you didn't had to code all that in C++

## Usage

 - activate venv if it's not already
    `source ./venv/bin/activate`
    OR
    `. ./venv/bin/activate`

 - Start the Elevation server in development mode (See `README.md` under `project/server`) :
    `./elevation -a`

 - run all test:
    `python -m unittest --verbose`

