#!/usr/bin/env python3

import requests
import unittest
import sys

from requests.exceptions import ConnectionError

BASE_PATH = 'http://127.0.0.1:'
port_no = '8080'

mock_ip = '123.2.2.2'
mock_mac = '21:22:F2:22:22:FF'
mock_name = 'Martha Stewart'

class UserApiTestCase(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)

    def test__good_request(self):
        """
        Test the return value of a proper request to 'usager/identification
        """
        req = None
        try:
            req = requests.post(BASE_PATH + port_no + '/usager/identification',
                    json={'ip': mock_ip, 'mac': mock_mac, 'name': mock_name})
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        self.assertEqual(req.status_code, 200, "status code is correct")

    def test__bad_request(self):
        req = None
        try:
            req = requests.post(BASE_PATH + port_no + '/usager/identification',
                    json={'spam': 'egg', 'egg': 'more egg', 'foo': 'baz'})
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        self.assertEqual(req.status_code, 400, "status code is correct")

    def test__response_format(self):
        req = None
        try:
            req = requests.post(BASE_PATH + port_no + '/usager/identification',
                    json={'ip': mock_ip, 'mac': mock_mac, 'name': mock_name})
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")

        json = req.json()
        self.assertTrue("identificateur" in json, "returns an identificateur")
        self.assertTrue("message" in json, "returns a message")


if __name__ == '__main__':
    if len(sys.argv) == 2:
        port_no = str(sys.argv.pop(1))
        print(port_no)
    else:
        port_no = '8080'
    unittest.main()

