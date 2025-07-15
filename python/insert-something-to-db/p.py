import pymysql
import pandas as pd

RDS_HOST=""
RDS_USER=""
RDS_PASSWORD=""
RDS_DB=""
RDS_CHARSET=""

conn=pymysql.connect(host=RDS_HOST,user=RDS_USER,db=RDS_DB,password=RDS_PASSWORD,charset=RDS_CHARSET)
cursor=conn.cursor(pymysql.cursors.DictCursor)

df=pd.read_csv("data.csv")

df_columns=df.columns

for idx,row in df.iterrows():
    conn=pymysql.connect(host=RDS_HOST,user=RDS_USER,db=RDS_DB,password=RDS_PASSWORD,charset=RDS_CHARSET)
    cursor=conn.cursor(pymysql.cursors.DictCursor)

    sql=f""
    print(sql)
    cursor.execute(sql)
    rows=cursor.fetchall()
    tc_id=rows[0]['id']

    sql=f""
    print(sql)
    cursor.execute(sql)
    conn.commit()
    conn.close()

print(df)

conn.commit()

conn.close()

