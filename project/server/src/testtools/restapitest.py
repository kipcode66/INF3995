import requests
import unittest

from requests.exceptions import ConnectionError


class TestRestApi(unittest.TestCase):

    def test__usager_identification(self):
        req = None
        try:
            req = requests.get('http://127.0.0.1:8080/usager/identification')
        except:
            print("The server could not be reached")
        self.assertIsNotNone(req, "No connection established")
        self.assertEqual(req.status_code, 200, "request was successfully received")


if __name__ == '__main__':
    unittest.main()
