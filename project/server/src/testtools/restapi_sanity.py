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
            req = requests.get(self.BASE_PATH + '/usager/identification')
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertEqual(req.status_code, 200, "wrong status code received")

    def test__get_usager_file(self):
        ID = 1
        req = None
        try:
            req = requests.get(self.BASE_PATH + '/usager/file/' + str(ID))
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertIn(req.status_code, [200, 403,], "received status code: " + str(req.status_code))

    def test__post_usager_chanson(self):
        ID = 1
        req = None
        try:
            req = requests.post(self.BASE_PATH + '/usager/chanson/' + str(ID))
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertIn(req.status_code, [200, 403, 413, 415,], "received status code: " + str(req.status_code))

    def test__delete_usager_chanson(self):
        ID = 1
        SONG_ID = 1
        req = None
        try:
            req = requests.delete(self.BASE_PATH + '/usager/chanson/' + str(ID) + '/' + str(SONG_ID))
        except ConnectionError:
            self.assertIsNotNone(req, "No connection established")
        self.assertIn(req.status_code, [200, 403, 405,], "received status code: " + str(req.status_code))


if __name__ == '__main__':
    unittest.main()
