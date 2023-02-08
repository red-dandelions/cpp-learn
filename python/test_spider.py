import requests
import bs4
def connect(url):
  x = requests.get(url)
  soup = bs4.BeautifulSoup(x.text, 'html.parser')
  ret=soup.find_all('script', id='__APP_DATA')
  print(ret)

def main(url):
  connect(url)

if __name__ == '__main__':
  url = "https://www.binance.com/zh-CN/support/announcement/c-48?navId=48"
  main(url)