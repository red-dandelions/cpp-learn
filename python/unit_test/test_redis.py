import unittest
import redis

class TestRedis(unittest.TestCase):
  def test_connect(self):
    pass

  def test_string(self):
    pass

def suite():
  return unittest.TestLoader().loadTestsFromTestCase(TestRedis)

if __name__ == '__main__':
  unittest.TextTestRunner().run(suite())