import redis

def main():
  # 连接 redis
  #
  # 1. 普通方式 (db=0，选择数据库0，redis 默认有 16 个数据库。如果设置了密码，则加上参数 password="xxx")
  redis_conn = redis.Redis(host="redis", port=6379, db=0)
  # 2. 连接池方式
  conn_pool = redis.ConnectionPool(host="redis", port=6379, db=0)
  redis_conn = redis.Redis(connection_pool=conn_pool)

  # 切换数据库
  #redis_conn.select()

  # 关闭连接
  #redis_conn.close()

  # 创建当前数据库的备份
  #redis_conn.save()
  # 异步的备份
  #redis_conn.bgsave()
  #将 Redis 备份文件（dump.rdb）移动到 Redis 目录中并启动服务器就可以恢复 Redis 数据。
  
  # 字符串
  redis_conn.set()          # 设置指定 key 的值
  redis_conn.get()          # 获取指定 key 的值
  redis_conn.getrange()     # 返回 key 中字符串值的子字符
  redis_conn.getset()       # 将给定 key 的值设为 value ，并返回 key 的旧值 ( old value )
  redis_conn.getbit()       # 对 key 所储存的字符串值，获取指定偏移量上的位 ( bit )
  redis_conn.mget()         # 获取所有(一个或多个)给定 key 的值
  redis_conn.setbit()       # 对 key 所储存的字符串值，设置或清除指定偏移量上的位(bit)
  redis_conn.setex()        # 设置 key 的值为 value 同时将过期时间设为 seconds
  redis_conn.setnx()        # 只有在 key 不存在时设置 key 的值
  redis_conn.setrange()     #	从偏移量 offset 开始用 value 覆写给定 key 所储存的字符串值
  redis_conn.strlen()       #	返回 key 所储存的字符串值的长度
  redis_conn.mset()         # 同时设置一个或多个 key-value 对
  redis_conn.msetnx()       #	同时设置一个或多个 key-value 对
  redis_conn.psetex()       #	以毫秒为单位设置 key 的生存时间
  redis_conn.incr()         #	将 key 中储存的数字值增一
  redis_conn.incrby()       #	将 key 所储存的值加上给定的增量值 ( increment )
  redis_conn.incrbyfloat()  #	将 key 所储存的值加上给定的浮点增量值 ( increment )
  redis_conn.decr()         #	将 key 中储存的数字值减一
  redis_conn.decrby()       #	将 key 所储存的值减去给定的减量值 ( decrement )
  redis_conn.append()       #	将 value 追加到 key 原来的值的末尾

  # 哈希
  redis_conn.hdel()         #	删除一个或多个哈希表字段
  redis_conn.hexists()      #	查看哈希表 key 中，指定的字段是否存在
  redis_conn.hget()         #	获取存储在哈希表中指定字段的值
  redis_conn.hgetall()      # 获取在哈希表中指定 key 的所有字段和值
  redis_conn.hincrby()      #	为哈希表 key 中的指定字段的整数值加上增量 increment
  redis_conn.hincrbyfloat() # 为哈希表 key 中的指定字段的浮点数值加上增量 increment
  redis_conn.hkeys()        #	获取所有哈希表中的字段
  redis_conn.hlen()         #	获取哈希表中字段的数量
  redis_conn.hmget()        #	获取所有给定字段的值
  redis_conn.hmset()        #	同时将多个 field-value (域-值)对设置到哈希表 key 中
  redis_conn.hset()         #	将哈希表 key 中的字段 field 的值设为 value
  redis_conn.hsetnx()       #	只有在字段 field 不存在时，设置哈希表字段的值
  redis_conn.hvals()        #	获取哈希表中所有值
  redis_conn.hscan()        #	迭代哈希表中的键值对
  redis_conn.hstrlen()      #	返回哈希表 key 中， 与给定域 field 相关联的值的字符串长度
 
  # 列表
  redis_conn.blpop()      # 移出并获取列表的第一个元素
  redis_conn.brpop()      # 移出并获取列表的最后一个元素
  redis_conn.brpoplpush() #	从列表中弹出一个值，并将该值插入到另外一个列表中并返回它
  redis_conn.lindex()     #	通过索引获取列表中的元素
  redis_conn.linsert()    #	在列表的元素前或者后插入元素
  redis_conn.llen()       #	获取列表长度
  redis_conn.lpop()       #	移出并获取列表的第一个元素
  redis_conn.lpush()      #	将一个或多个值插入到列表头部
  redis_conn.lpushx()     #	将一个值插入到已存在的列表头部
  redis_conn.lrange()     #	获取列表指定范围内的元素
  redis_conn.lrem()       #	移除列表元素
  redis_conn.lset()       #	通过索引设置列表元素的值
  redis_conn.ltrim()      #	对一个列表进行修剪(trim)
  redis_conn.rpop()       #	移除并获取列表最后一个元素
  redis_conn.rpoplpush()  # 移除列表的最后一个元素，并将该元素添加到另一个列表并返回
  redis_conn.rpush()      #	在列表中添加一个或多个值
  redis_conn.rpushx()     #	为已存在的列表添加值

  # 集合 set
  redis_conn.sadd()         #	向集合添加一个或多个成员
  redis_conn.scard()        #	获取集合的成员数
  redis_conn.sdiff()        #	返回给定所有集合的差集
  redis_conn.sdiffstore()   #	返回给定所有集合的差集并存储在 destination 中
  redis_conn.sinter()       #	返回给定所有集合的交集
  redis_conn.sinterstore()  #	返回给定所有集合的交集并存储在 destination 中
  redis_conn.sismember()    #	判断 member 元素是否是集合 key 的成员
  redis_conn.smembers()     #	返回集合中的所有成员
  redis_conn.smove()        #	将 member 元素从 source 集合移动到 destination 集合
  redis_conn.spop()         #	移除并返回集合中的一个随机元素
  redis_conn.srandmember()  #	返回集合中一个或多个随机数
  redis_conn.srem()         #	移除集合中一个或多个成员
  redis_conn.sunion()       #	返回所有给定集合的并集
  redis_conn.sunionstore()  #	所有给定集合的并集存储在 destination 集合中
  redis_conn.sscan()        #	迭代集合中的元素

  # 有序集合
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

  # hyperloglog
  redis_conn.pfadd()    #	添加指定元素到 HyperLogLog 中。
  redis_conn.pfcount()  #	返回给定 HyperLogLog 的基数估算值。
  redis_conn.pfmerge()  #	将多个 HyperLogLog 合并为一个 HyperLogLog

  # 地理信息
  redis_conn.geoadd()

  # stream
  redis_conn.xadd()

  # pub/sub
  pub_sub_obj = redis_conn.pubsub() # 获取 pub/sub 对象
  pub_sub_obj.psubscribe() # 订阅一个或多个符合给定模式的频道。
  redis_conn.publish() # 将信息发送到指定的频道。
  pub_sub_obj.punsubscribe() # 退订所有给定模式的频道。
  pub_sub_obj.subscribe() #	订阅给定的一个或多个频道的信息。
  pub_sub_obj.unsubscribe() # 指退订给定的频道。
  pub_sub_obj.get_message() # 获取消息

  # pipeline
  p = redis_conn.pipeline(transaction=False)
  p.sadd()
  # ...
  p.execute()
  p = redis_conn.pipeline()
  p.watch()
  p.multi()
  p.sadd()
  p.execute()


if __name__ == '__main__':
  main()