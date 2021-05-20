from urllib import request,error
import re,os
import chardet   #需要导入这个模块，检测编码格式
k = ["红胜","黑胜","和局"]
j = {t:list() for t in k}
for i in range(11000,12142):
    url = "https://www.xqbase.com/xqbase?gameid="+str(i)
    try:
        response = request.urlopen(url)
    except error.HTTPError as e:
        print(e.reason, e.code, e.headers, sep='\n')
    except ValueError:
        print("please input a right url")
    else:
        html_code = response.read()
        encode_type = chardet.detect(html_code)  
        html_code = html_code.decode("GBK") #进行相应解码，赋给原标识符（变量）
        result = re.findall("<pre>.*</pre>",html_code,flags = re.S)
        if len(result):
            x = result[0][5:-6].split("\u3000\u3000")
            pgn = x[0].split("\n")
            pgn.append(x[1][1:-1])
            
            j[pgn[-1]].append(str(len(j[pgn[-1]])+1)+".txt")
            f = open("test/"+pgn[-1]+'/'+str(len(j[pgn[-1]])+11000)+".txt","w")
            for p in pgn[:-1]:
                f.write(p)
            f.close()
            print(i,"complete",pgn[-1],len(j[pgn[-1]]))
        else:
            print(i,'null')
