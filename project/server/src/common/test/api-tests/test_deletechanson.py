#!/usr/bin/env python3

import requests
import unittest
import sys

from requests.exceptions import ConnectionError

BASE_PATH = 'http://127.0.0.1:'
port_no = '8080'

MOCK_ID = 425
MOCK_CHANSON_NO = 3


class UsagerChanson_TestCase(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)


    @unittest.skip("Broken")
    def test__return_code_sanity(self):
        req = None
        try:
            req = requests.delete(BASE_PATH + port_no + '/usager/chanson/'
                    + str(MOCK_ID) + '/' + str(MOCK_CHANSON_NO))
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        self.assertIn(req.status_code, [200, 403, 405], "status code makes sense")


if __name__ == '__main__':
    if len(sys.argv) == 2:
        port_no = str(sys.argv.pop(1))
        print(port_no)
    else:
        port_no = '8080'
    unittest.main()

