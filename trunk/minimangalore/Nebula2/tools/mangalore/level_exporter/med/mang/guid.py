#-------------------------------------------------------------------------------
# guid.py
#-------------------------------------------------------------------------------

import time, random, socket, md5

def guid():
    t = long(time.time()*1000)
    r = long(random.random()*1000000000000000000L)
    ip = ''
    try:
        ip = socket.gethostbyname(socket.gethostname())
    except:
        ip = str(random.random()*1000000000000000000L)
    data = str(t) + ' ' + str(r) + ip
    hd = md5.new(data).hexdigest()
    guid = '%s-%s-%s-%s-%s' % (hd[:8], hd[8:12], hd[12:16], hd[16:20], hd[20:])
    return guid

#-------------------------------------------------------------------------------
# Eof
#-------------------------------------------------------------------------------