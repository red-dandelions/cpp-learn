import pymysql
import unittest

class TestMysql(unittest.TestCase):
  def test_connect(self):
    db = pymysql.connect(host='mysql', user='',password='',database='test')

    cursor = db.cursor()

    sql = ''

    try:
      cursor.execute(sql)
      
      results = cursor.fetchall()
      for row in results:
        print(row)

      db.commit()
    except:
      db.rollback()
    db.close()

def suite():
  return unittest.TestLoader().loadTestsFromTestCase(TestMysql)

if __name__ == '__main__':
  unittest.TextTestRunner().run(suite())