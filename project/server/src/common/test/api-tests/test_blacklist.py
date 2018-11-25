#!/usr/bin/env python3

import unittest
import sys
import requests
import subprocess
import shlex

from requests.exceptions import ConnectionError
from requests.packages.urllib3 import disable_warnings


BASE_PATH = 'https://127.0.0.1:'
port_no = '4433'

mock_token = "123456"
mock_headers = {"X-Auth-Token": mock_token}
mock_ip = "123.2.3.4"
mock_mac = "FF:12:34:56:89:0F"
mock_nom = "test_user"

endpoints = {
    'identification': {
        'request': requests.post,
        'path': '/usager/identification',
        'data': {
            'ip': mock_ip,
            'mac':mock_mac,
            'name': mock_nom,
        }
    },
    'listenoire': {
        'request': requests.get,
        'path': '/superviseur/listenoire',
    },
    'bloquer': {
        'request': requests.post,
        'path': '/superviseur/bloquer',
        'headers': mock_headers,
        'data': {
            'MAC': mock_mac,
            'ip' : mock_ip,
            'nom': mock_nom
        },
    },
}

class BlacklistTest(unittest.TestCase):
    """
    Testing the blacklist. We are using curls because the python library didn't work
    over HTTPS because of our custom SSL daemon """

    def __init__(self, *args, **kwargs):
        super(self.__class__, self).__init__(*args, **kwargs)

    @classmethod
    def setUpClass(self):
        "Login as admin"
        try:
            cmd = 'curl -k -X POST https://127.0.0.1:4433/superviseur/login '\
                  '-H \'X-Auth-Token: ' + mock_token + '\' '\
                  '--data \'{"usager": "admin", "mot_de_passe":"admin"}\''
            args = shlex.split(cmd)
            process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            stdout, _ = process.communicate()
            self.assertTrue(
                    "succesful" in str(stdout) or
                    "already"   in str(stdout),
                    "generated user identification")
        except:
            self.assertTrue(False, "Could not login as Admin")


    def test_listenoire_good(self):
        "Test getting the blacklist"

        cmd = "curl -k -X GET https://127.0.0.1:4433/superviseur/listenoire -H 'X-Auth-Token: " + mock_token + "' "
        args = shlex.split(cmd)
        process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, _ = process.communicate()
        self.assertIn("bloque", str(stdout), "successfully retrieved blacklist")


    def test_bloquer(self):
        "Creates a user and then block it"
        disable_warnings()
        cmd = 'curl -k -X POST https://127.0.0.1:' + port_no + '/usager/identification --data '\
              '\'{"ip": "' + mock_ip + '", "mac":"' + mock_mac + '", "nom": "test_user1"}\''
        args = shlex.split(cmd)
        process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, _ = process.communicate()
        self.assertTrue("identificateur" in str(stdout), "generated user identification")

        cmd = "curl -k -X POST https://127.0.0.1:" + port_no + "/superviseur/bloquer "\
              "-H 'X-Auth-Token: " + mock_token + \
              "' --data '{\"mac\":\"" + mock_mac + "\", \"ip\": \"\", \"nom\": \"\"}' "
        args = shlex.split(cmd)
        process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, _ = process.communicate()
        self.assertIn(
                "blocked", str(stdout), "user succesfully blocked")

    def test_bloquer_debloquer(self):
        "Creates a user and then block it"
        disable_warnings()
        cmd = 'curl -k -X POST https://127.0.0.1:' + port_no + '/usager/identification --data '\
              '\'{"ip": "' + mock_ip + '", "mac":"' + mock_mac + '", "nom": "test_user1"}\''\
              " -w \"\n%{http_code}\""
        args = shlex.split(cmd)
        process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, _ = process.communicate()
        self.assertTrue("200" in str(stdout), "generated user identification")

        cmd = "curl -k -X POST https://127.0.0.1:" + port_no + "/superviseur/bloquer "\
              "-H 'X-Auth-Token: " + mock_token + \
              "' --data '{\"mac\":\"" + mock_mac + "\", \"ip\": \"\", \"nom\": \"\"}' "\
              " -w \"\n%{http_code}\""
        args = shlex.split(cmd)
        process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, _ = process.communicate()
        self.assertIn(
                "200", str(stdout), "user succesfully blocked")

        cmd = "curl -k -X POST https://127.0.0.1:" + port_no + "/superviseur/debloquer "\
              "-H 'X-Auth-Token: " + mock_token + \
              "' --data '{\"mac\":\"" + mock_mac + "\", \"ip\": \"\", \"nom\": \"\"}' "\
              " -w \"\n%{http_code}\""
        args = shlex.split(cmd)
        process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, _ = process.communicate()
        self.assertIn(
                "200", str(stdout), "user succesfully blocked")

if __name__ == '__main__':
    if len(sys.argv) == 2:
        port_no = str(sys.argv.pop(1))
        print(port_no)
    else:
        port_no = '4433'
    unittest.main()

