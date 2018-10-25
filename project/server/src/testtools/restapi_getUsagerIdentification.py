#!/usr/bin/env python3

import requests
import unittest
import sys

from requests.exceptions import ConnectionError

port_no = '8080'
BASE_PATH = 'http://127.0.0.1:' + port_no
mock_ip = '123.2.2.2'
mock_mac = '22:22:22:22:22:22'
mock_name = 'Bob Ross'

class TestRestApiSanity(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)

    def test__sanity_good_request(self):
        req = None
        try:
            req = requests.get(BASE_PATH + '/usager/identification',
                    json={'ip': mock_ip, 'mac': mock_mac, 'name': mock_name})
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertEqual(req.status_code, 200, "wrong status code received")

    def test__get_usager_bad_request(self):
        req = None
        try:
            req = requests.get(BASE_PATH + '/usager/identification',
                    json={'spam': 'egg', 'egg': 'more egg', 'foo': 'baz'})
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertEqual(req.status_code, 400, "wrong status code received")

    def test__get_usager_response(self):
        req = None
        try:
            req = requests.get(BASE_PATH + '/usager/identification',
                    json={'ip': mock_ip, 'mac': mock_mac, 'name': mock_name})
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertEqual(req.status_code, 200, "wrong status code received")
        print(req.content)

if __name__ == '__main__':
    # if len(sys.argv) == 2:
    #     # print('var=' + sys.argv[1])
    #     port_no = str(sys.argv[1])
    unittest.main()
