#coding=UTF-8
import unittest
import redis
import time

class TestRedis(unittest.TestCase):
  # redis 连接
  def test_connect(self):
    # 1. 普通方式 (db=0，选择数据库0，redis 默认有 16 个数据库。如果设置了密码，则加上参数 password="xxx")
    conn = redis.Redis(host="redis", port=6379, db=0)
    self.assertTrue(conn.ping())

    # 2. 连接池方式
    conn_pool = redis.ConnectionPool(host="redis", port=6379, db=0)
    redis_conn = redis.Redis(connection_pool=conn_pool)
    self.assertTrue(redis_conn.ping())

    conn.close()
    redis_conn.close()

  # redis 字符串
  def test_string(self):
    redis_conn = redis.Redis(host="redis", port=6379, db=0)

    # 设置指定 key 的值
    ret = redis_conn.set("test_string", "value1")
    self.assertTrue(ret)

    # 获取指定 key 的值
    ret = redis_conn.get("test_string")
    self.assertTrue(ret == b"value1")

    # 返回 key 中字符串值的子字符
    ret = redis_conn.getrange("test_string", 0, 2)
    self.assertTrue(ret == b'val')

    # 将给定 key 的值设为 value ，并返回 key 的旧值 ( old value )
    ret = redis_conn.getset('test_string', 'value2')
    self.assertTrue(ret == b'value1')
    ret = redis_conn.get('test_string')
    self.assertTrue(ret == b'value2')

    # 同时设置一个或多个 key-value 对
    kv = {
      'key_1': 'value1',
      'key_2': 'value2'
    }
    ret = redis_conn.mset(kv)
    self.assertTrue(ret)

    # 获取所有(一个或多个)给定 key 的
    ret = redis_conn.mget('key_1', 'key_2')
    self.assertTrue(ret[0] == b'value1')
    self.assertTrue(ret[1] == b'value2')

    # 只有在 key 不存在时设置 key 的值
    ret = redis_conn.setnx('test_string', 'value3')
    self.assertFalse(ret)

    # 设置 key 的值为 value 同时将过期时间设为 seconds
    ret = redis_conn.setex('setex', 1, 'value')
    self.assertTrue(ret)
    ret = redis_conn.get('setex')
    self.assertTrue(ret == b'value')
    time.sleep(1)
    ret = redis_conn.get('setex')
    self.assertTrue(ret == None)

    #	从偏移量 offset 开始用 value 覆写给定 key 所储存的字符串值，返回更新后的字符串长度
    ret = redis_conn.setrange(name='test_string', offset=1, value='value')
    self.assertTrue(ret == 6)

    #	返回 key 所储存的字符串值的长度
    redis_conn.strlen('test_string')
    self.assertTrue(ret == 6)

    #	同时设置一个或多个 key-value 对
    kv = {
      'test_string': 'value',
      'key_3': 'value'
    }
    ret = redis_conn.msetnx(kv)
    self.assertFalse(ret)
    #	以毫秒为单位设置 key 的生存时间
    ret = redis_conn.psetex('psetex', 500, 'value')
    self.assertTrue(ret)
    ret = redis_conn.get('psetex')
    self.assertTrue(ret == b'value')
    time.sleep(0.5)
    ret = redis_conn.get('psetex')
    self.assertTrue(ret == None)

    #	将 key 中储存的数字值增一
    redis_conn.set(name='incr', value=0)
    ret = redis_conn.incr('incr')
    self.assertTrue(ret == 1)
    #	将 key 所储存的值加上给定的增量值 ( increment )
    ret = redis_conn.incrby('incr', 3)
    self.assertTrue(ret == 4)
    #	将 key 所储存的值加上给定的浮点增量值 ( increment )
    ret = redis_conn.incrbyfloat('incr', 0.3)
    self.assertTrue(ret == 4.3)
    #	将 key 中储存的数字值减一
    redis_conn.set('incr', 4)
    ret = redis_conn.decr('incr')
    self.assertTrue(ret == 3)
    #	将 key 所储存的值减去给定的减量值 ( decrement )
    ret = redis_conn.decrby('incr', 3)
    self.assertTrue(ret == 0)

    #	将 value 追加到 key 原来的值的末尾，返回更新后字符串的长度
    ret = redis_conn.append('test_string', 'append')
    self.assertTrue(ret == 12)

    redis_conn.close()

  # redis 哈希
  def test_hash(self):
    redis_conn = redis.Redis(host="redis", port=6379, db=0)

    if redis_conn.exists('key1'):
      redis_conn.delete('key1')

    #	将哈希表 key 中的字段 field 的值设为 value
    kv = {
      'skey1': 'value1',
      'skey2': 'value2'
    }
    ret = redis_conn.hset(name='key1', mapping=kv)
    self.assertTrue(ret == 2)

    #	查看哈希表 key 中，指定的字段是否存在
    ret = redis_conn.hexists('key1', 'skey1')
    self.assertTrue(ret)
    ret = redis_conn.hexists('key1', 'skey3')
    self.assertFalse(ret)

    #	获取存储在哈希表中指定字段的值
    ret = redis_conn.hget('key1', 'skey1')
    self.assertTrue(ret == b'value1')
    ret = redis_conn.hget('key1', 'skey3')
    self.assertTrue(ret == None)

    # 获取在哈希表中指定 key 的所有字段和值
    ret = redis_conn.hgetall('key1')
    self.assertTrue(ret[b'skey1'] == b'value1')
    self.assertTrue(ret[b'skey2'] == b'value2')

    #	获取所有哈希表中的字段
    ret = redis_conn.hkeys('key1')
    self.assertTrue(ret[0] == b'skey1')
    self.assertTrue(ret[1] == b'skey2')

    #	获取哈希表中字段的数量
    ret = redis_conn.hlen('key1')
    self.assertTrue(ret == 2)

    #	只有在字段 field 不存在时，设置哈希表字段的值
    ret = redis_conn.hsetnx('key1', 'skey4', 1)
    self.assertTrue(ret == 1)

    #	为哈希表 key 中的指定字段的整数值加上增量 increment
    ret = redis_conn.hincrby('key1', 'skey4', 1)
    self.assertTrue(ret == 2)
    # 为哈希表 key 中的指定字段的浮点数值加上增量 increment
    ret = redis_conn.hincrbyfloat('key1', 'skey4', -0.5)
    self.assertTrue(ret == 1.5)

    #	获取所有给定字段的值
    ret = redis_conn.hmget('key1', 'skey3', 'skey4')
    self.assertTrue(ret[0] == None)
    self.assertTrue(ret[1] == b'1.5')

    #	同时将多个 field-value (域-值)对设置到哈希表 key 中
    # 建议使用 hset
    # ret = redis_conn.hmset('key1', kv)

    #	获取哈希表中所有值
    ret = redis_conn.hvals('key1')
    self.assertTrue(ret[0] == b'value1')
    self.assertTrue(ret[1] == b'value2')
    self.assertTrue(ret[2] == b'1.5')

    #	返回哈希表 key 中， 与给定域 field 相关联的值的字符串长度
    ret = redis_conn.hstrlen('key1', 'skey4')
    self.assertTrue(ret == 3)

    #	迭代哈希表中的键值对
    #
    # count 不一定生效，只是一个提示
    ret = redis_conn.hscan(name='key1', cursor=0, match='skey*', count=2)
    self.assertTrue(ret[0] == 0)
    self.assertTrue(ret[1][b'skey1'] == b'value1')
    self.assertTrue(ret[1][b'skey2'] == b'value2')
    self.assertTrue(ret[1][b'skey4'] == b'1.5')

    #	删除一个或多个哈希表字段
    ret = redis_conn.hdel('key1', 'skey1', 'skey2', 'skey3')
    self.assertTrue(ret == 2)

    redis_conn.close()

  # redis 列表
  def test_list(self):
    redis_conn = redis.Redis(host='redis', port=6379, db=0)

    if redis_conn.exists('list'):
      redis_conn.delete('list')

    #	将一个或多个值插入到列表头部
    ret = redis_conn.lpush('list', 'value1', 'value2', 'value3', 'value6')
    self.assertTrue(ret == 4)
    #	将一个值插入到已存在的列表头部
    ret = redis_conn.lpushx('list', 'value4')
    self.assertTrue(ret == 5)
    #	在列表的元素前或者后插入元素
    ret = redis_conn.linsert('list','after','value2', 'value5')
    self.assertTrue(ret == 6)

    #	获取列表长度
    ret = redis_conn.llen('list')
    self.assertTrue(ret == 6)
    #	对一个列表进行修剪(trim)
    ret = redis_conn.ltrim('list', 0, 4)
    #	获取列表指定范围内的元素
    rets = redis_conn.lrange('list', 0, 6)
    self.assertTrue(rets[0] == b'value4')
    self.assertTrue(rets[1] == b'value6')
    self.assertTrue(rets[2] == b'value3')
    self.assertTrue(rets[3] == b'value2')
    self.assertTrue(rets[4] == b'value5')

    #	在列表中添加一个或多个值
    ret = redis_conn.rpush('list', 'value1')
    #	为已存在的列表添加值
    ret = redis_conn.rpushx('list', 'value1')
    ret = redis_conn.lpush('list', 'value1', 'value1', 'value1')
    #	移除列表元素
    ret = redis_conn.lrem('list', 2, 'value1')
    rets = redis_conn.lrange('list', 0, 10)
    self.assertTrue(rets[0] == b'value1')
    self.assertTrue(rets[1] == b'value4')
    self.assertTrue(rets[2] == b'value6')
    self.assertTrue(rets[3] == b'value3')
    self.assertTrue(rets[4] == b'value2')
    self.assertTrue(rets[5] == b'value5')
    self.assertTrue(rets[6] == b'value1')
    self.assertTrue(rets[7] == b'value1')

    ret = redis_conn.lrem('list', -1, 'value1')
    rets = redis_conn.lrange('list', 0, 10)
    self.assertTrue(rets[0] == b'value1')
    self.assertTrue(rets[1] == b'value4')
    self.assertTrue(rets[2] == b'value6')
    self.assertTrue(rets[3] == b'value3')
    self.assertTrue(rets[4] == b'value2')
    self.assertTrue(rets[5] == b'value5')
    self.assertTrue(rets[6] == b'value1')

    ret = redis_conn.lrem('list', 0, 'value1')
    rets = redis_conn.lrange('list', 0, 10)
    self.assertTrue(rets[0] == b'value4')
    self.assertTrue(rets[1] == b'value6')
    self.assertTrue(rets[2] == b'value3')
    self.assertTrue(rets[3] == b'value2')
    self.assertTrue(rets[4] == b'value5')

    # 移出并获取列表的第一个元素, value4
    ret0 = redis_conn.blpop('list')
    # 移出并获取列表的最后一个元素, value5
    ret1 = redis_conn.brpop('list')
    #	从列表中弹出一个值，并将该值插入到另外一个列表中并返回它, value6
    ret2 = redis_conn.brpoplpush('list','list')
    #	通过索引获取列表中的元素, value3
    ret3 = redis_conn.lindex('list', 0)
    # 移出并获取列表的第一个元素, value2
    redis_conn.lpop('list')
    ret4 = redis_conn.lpop('list')
    
    #	通过索引设置列表元素的值
    ret = redis_conn.lset('list', 0, 'value1')
    self.assertTrue(ret)

    # 移除列表的最后一个元素，并将该元素添加到另一个列表并返回
    ret = redis_conn.rpoplpush('list', 'list')
    self.assertTrue(ret == b'value1')

    #	移除并获取列表最后一个元素
    ret = redis_conn.rpop('list')
    self.assertTrue(ret == b'value1')

    redis_conn.close()

  # redis 集合
  def test_set(self):
    redis_conn = redis.Redis(host='redis', port=6379, db=0)

    if redis_conn.exists('set1'):
      redis_conn.delete('set1')
    if redis_conn.exists('set2'):
      redis_conn.delete('set2')

    #	向集合添加一个或多个成员
    ret = redis_conn.sadd('set1', 'value1', 'value2', 'value2')
    self.assertTrue(ret == 2)
    ret = redis_conn.sadd('set2', 'value1', 'value3', 'value3')
    self.assertTrue(ret == 2)

    #	获取集合的成员数
    ret = redis_conn.scard('set1')
    self.assertTrue(ret == 2)

    #	判断 member 元素是否是集合 key 的成员
    ret = redis_conn.sismember('set1', 'value2')
    self.assertTrue(ret)

    #	返回集合中的所有成员
    ret = redis_conn.smembers('set1')
    self.assertTrue(b'value1' in ret)
    self.assertTrue(b'value2' in ret)

    #	返回给定所有集合的差集 (set1 中没有 set2 的元素)
    ret = redis_conn.sdiff('set1', 'set2')
    self.assertTrue(b'value2' in ret)

    #	返回给定所有集合的交集
    ret = redis_conn.sinter('set1', 'set2')
    self.assertTrue(b'value1' in ret)

    #	返回所有给定集合的并集
    ret = redis_conn.sunion('set1', 'set2')
    self.assertTrue(b'value1' in ret)
    self.assertTrue(b'value2' in ret)
    self.assertTrue(b'value3' in ret)

    # redis_conn.sdiffstore()   #	返回给定所有集合的差集并存储在 destination 中
    # redis_conn.sinterstore()  #	返回给定所有集合的交集并存储在 destination 中
    #	所有给定集合的并集存储在 destination 集合中
    ret3 = redis_conn.sunionstore('set3', 'set1', 'set2')

    #	将 member 元素从 source 集合移动到 destination 集合
    ret = redis_conn.smove('set1', 'set2', 'value2')
    self.assertTrue(ret)
    #	迭代集合中的元素
    ret = redis_conn.sscan('set3', 0, 'value*', 2)
    self.assertTrue(ret[0] == 1)
    self.assertTrue(ret[1][0] == b'value1')
    self.assertTrue(ret[1][1] == b'value2')
    ret = redis_conn.sscan('set3', ret[0], 'value*', 2)
    self.assertTrue(ret[0] == 0)
    self.assertTrue(ret[1][0] == b'value3')
    
    #	移除并返回集合中的一个随机元素
    ret = redis_conn.spop('set1')
    self.assertTrue(ret == b'value1')
    #	返回集合中一个或多个随机数
    ret = redis_conn.srandmember('set2', 2)
    self.assertTrue(len(ret) == 2)
    #	移除集合中一个或多个成员
    ret = redis_conn.srem('set3', 'value1', 'value2', 'value3')
    self.assertTrue(ret == 3)

    redis_conn.close()

  # redis 有序集合
  def test_sort_set(self):
    pass
    '''
    ZADD	向有序集合添加一个或多个成员，或者更新已存在成员的分数
    ZCARD	获取有序集合的成员数
    ZCOUNT	计算在有序集合中指定区间分数的成员数
    ZINCRBY	有序集合中对指定成员的分数加上增量 increment
    ZINTERSTORE	计算给定的一个或多个有序集的交集并将结果集存储在新的有序集合 key 中
    ZLEXCOUNT	在有序集合中计算指定字典区间内成员数量
    ZRANGE	通过索引区间返回有序集合成指定区间内的成员
    ZRANGEBYLEX	通过字典区间返回有序集合的成员
    ZRANGEBYSCORE	通过分数返回有序集合指定区间内的成员
    ZRANK	返回有序集合中指定成员的索引
    ZREM	移除有序集合中的一个或多个成员
    ZREMRANGEBYLEX	移除有序集合中给定的字典区间的所有成员
    ZREMRANGEBYRANK	移除有序集合中给定的排名区间的所有成员
    ZREMRANGEBYSCORE	移除有序集合中给定的分数区间的所有成员
    ZREVRANGE	返回有序集中指定区间内的成员，通过索引，分数从高到底
    ZREVRANGEBYSCORE	返回有序集中指定分数区间内的成员，分数从高到低排序
    ZREVRANK	返回有序集合中指定成员的排名，有序集成员按分数值递减(从大到小)排序
    ZSCORE	返回有序集中，成员的分数值
    ZUNIONSTORE	计算一个或多个有序集的并集，并存储在新的 key 中
    ZSCAN	迭代有序集合中的元素（包括元素成员和元素分值）
    '''

  # redis hyperloglog
  def test_hyperloglog(self):
    redis_conn = redis.Redis(host='redis', port=6379, db=0)

    if redis_conn.exists('hyper1'):
      redis_conn.delete('hyper1')
    if redis_conn.exists('hyper2'):
      redis_conn.delete('hyper2')

    #	添加指定元素到 HyperLogLog 中。
    ret = redis_conn.pfadd('hyper1', 'value1', 'value2', 'value3','value4','value1','value2')
    self.assertTrue(ret == 1)
    #	返回给定 HyperLogLog 的基数估算值。
    ret = redis_conn.pfcount('hyper1')
    self.assertTrue(ret == 4)
    #	将多个 HyperLogLog 合并为一个 HyperLogLog
    redis_conn.pfadd('hyper2', 'value5','value6')
    ret = redis_conn.pfmerge('hyper1', 'hyper2')
    self.assertTrue(ret)
    ret = redis_conn.pfcount('hyper1')
    self.assertTrue(ret == 6)

    redis_conn.close()

  # redis pipeline
  def test_pipeline(self):
    conn = redis.Redis(host='redis',port=6379,db=0)
    conn.set('pipeline', 'value1')
    # transaction=False 则为流水线
    with conn.pipeline(transaction=True) as p:
      p.watch('pipeline')
      # 开启事物
      p.multi()

      # 命令入队
      p.get('pipeline')

      p.set('pipeline', 'value2')

      p.get('pipeline')

      # 执行事务
      ret = p.execute()

    self.assertTrue(ret[0] == b'value1')
    self.assertTrue(ret[1])
    self.assertTrue(ret[2] == b'value2')

    if conn.exists('pipeline'):
      conn.delete('pipeline')

    conn.close()

def suite():
  return unittest.TestLoader().loadTestsFromTestCase(TestRedis)

if __name__ == '__main__':
  unittest.TextTestRunner().run(suite())