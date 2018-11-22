#!/usr/bin/env python3

import unittest
import sys
import requests

from requests.exceptions import ConnectionError
from requests.packages.urllib3 import disable_warnings


BASE_PATH = 'https://127.0.0.1:'
port_no = '4433'

mock_headers = {"X-Auth-Token": "123"}
mock_ip = "123.2.3.4"
mock_mac = "FF:12:34:56:89:0F"
mock_nom = "test_user"

endpoints = {
    'identification': {
        'request': requests.post,
        'path': '/usager/identification',
        'headers': mock_headers,
        'data': {
            'ip': mock_ip,
            'mac':mock_mac,
            'nom': mock_nom,
        }
    },
    'listenoire': {
        'request': requests.get,
        'path': '/superviseur/listenoire',
    },
    'bloquer': {
        'request': requests.post,
        'path': '/superviseur/bloquer',
        'data': {"MAC": mock_mac},
    },
}

class BlacklistTest(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)

    @classmethod
    def setUpClass(self):
        "Login as admin"
        disable_warnings()
        try:
            req = requests.post(
                    BASE_PATH + port_no + '/superviseur/login',
                    data={"usager": "admin", "mot_de_passe": "admin"},
                    headers=mock_headers,
                    verify=False)
        except:
            assertTrue(False, "Could not login as Admin")


    def test_getListenoire_good(self):
        "Test getting the blacklist"

        disable_warnings()
        e = endpoints['listenoire']
        req = None
        try:
            req = e['request'](
                BASE_PATH + port_no + e['path'],
                headers=mock_headers,
                verify=False)
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")
        self.assertEqual(req.status_code, 200, "status code is correct")


    @unittest.skip("WIP")
    def test__postBloquer(self):
        "Creates a user and then block it"
        disable_warnings()
        req = None
        try:
            e = endpoints['identification']
            create_req = e['request'](
                BASE_PATH + port_no + e['path'],
                headers=e['headers'],
                verify=False,
                data=e['data'])

            e = endpoints['bloquer']
            block_req = e['request'](
                BASE_PATH + port_no + e['path'],
                headers=mock_headers,
                data=e['data'],
                verify=False)
        except ConnectionError:
            self.assertIsNotNone(req, "Connection established")

        self.assertEqual(create_req.status_code, 200, "could not create user")
        self.assertEqual(block_req.status_code, 200, "could not block user")

if __name__ == '__main__':
    if len(sys.argv) == 2:
        port_no = str(sys.argv.pop(1))
        print(port_no)
    else:
        port_no = '4433'
    unittest.main()

