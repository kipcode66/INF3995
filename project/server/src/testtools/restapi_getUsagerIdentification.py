#!/usr/bin/env python3

import requests
import unittest
import sys

from requests.exceptions import ConnectionError

BASE_PATH = 'http://127.0.0.1:'
mock_ip = '123.2.2.2'
mock_mac = '21:22:F2:22:22:FF'
mock_name = 'Martha Stewart'

class UsagerIdentification_TestCase(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)

    def test__good_request(self):
        req = None
        try:
            req = requests.get(BASE_PATH + port_no + '/usager/identification',
                    json={'ip': mock_ip, 'mac': mock_mac, 'name': mock_name})
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        self.assertEqual(req.status_code, 200, "status code is correct")

    def test__bad_request(self):
        req = None
        try:
            req = requests.get(BASE_PATH + port_no + '/usager/identification',
                    json={'spam': 'egg', 'egg': 'more egg', 'foo': 'baz'})
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        self.assertEqual(req.status_code, 400, "status code is correct")

    def test__response_format(self):
        req = None
        try:
            req = requests.get(BASE_PATH + port_no + '/usager/identification',
                    json={'ip': mock_ip, 'mac': mock_mac, 'name': mock_name})
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        json = req.json()
        # print(json)
        self.assertTrue('identificateur' in json, "field 'identificateur' in response")
        self.assertTrue('message' in json, "field 'message' in response")
        self.assertTrue('success' in json["message"], "response message indicate 'uccess'")

if __name__ == '__main__':
    global port_no

    if len(sys.argv) == 2:
        port_no = str(sys.argv.pop(1))
        print(port_no)
    else:
        port_no = '8080'
    unittest.main()

