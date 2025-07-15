import pandas as pd
import pymysql

summary_df = pd.read_excel('hint.xlsx',usecols=[0,1,2,3,4],sheet_name="summary")
summary_df

sheet_complete_stat = summary_df[summary_df['status']=="done"]
sheet_complete_stat

sheet_complete_names = list(sheet_complete_stat['sheet name'])
print(sheet_complete_names)

with pymysql.connect(host="", user="", passwd="", db="", charset='') as conn:
    with conn.cursor() as cur:
        for sheet_name in sheet_complete_names:
            df = pd.read_excel('hint.xlsx', header=None,
                               sheet_name=sheet_name)
            for i in range(len(df)):
                sql = "INSERT INTO SomeWhere(`id`,`order`,`language`,`text`) VALUE(%s,0,1,%s)"
                cur.execute(sql, (df.iloc[i][0], df.iloc[i][1]))
            conn.commit()
