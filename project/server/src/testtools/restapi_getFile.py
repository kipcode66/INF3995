#!/usr/bin/env python3

import requests
import unittest
import sys

from requests.exceptions import ConnectionError

BASE_PATH = 'http://127.0.0.1:'
MOCK_ID = 425

class UsagerIdentification_TestCase(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)

    def test__return_code_sanity(self):
        req = None
        try:
            req = requests.get(BASE_PATH + port_no + '/usager/file/' + str(MOCK_ID))
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        self.assertIn(req.status_code, [200, 403, 500], "status code makes sense")


if __name__ == '__main__':
    global port_no

    if len(sys.argv) == 2:
        port_no = str(sys.argv.pop(1))
        print(port_no)
    else:
        port_no = '8080'
    unittest.main()

