import urllib2
import urllib
import json

url = 'http://localhost/ts/transactionServer.php'
method = 'getSubaccountInfo'
acc_token = '123'
testing = 'true'
sub_id = '54befaaf6bdaa77409000038'

params = urllib.urlencode({'method':method, 'acc_token':acc_token, 'testing':testing, 'subId':sub_id})

r = urllib2.urlopen('%s?%s'%(url,params))
response = r.read()
subaccount = json.loads(response)
print subaccount['message']
