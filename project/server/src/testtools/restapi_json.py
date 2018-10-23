#!/usr/bin/env python3

import requests
import unittest

from requests.exceptions import ConnectionError


class TestRestApiSanity(unittest.TestCase):

    BASE_PATH = 'http://127.0.0.1:8080'

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)

    def test__get_usager_identification(self):
        req = None
        try:
            req = requests.get(self.BASE_PATH + '/usager/identification', 
                    json={'ip': '123.2.2.2', 'mac': 'BB:BB:BB:BB:BB:BB', 'name': 'bob ross'})
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertEqual(req.status_code, 200, "wrong status code received")


if __name__ == '__main__':
    unittest.main()
