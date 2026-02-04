import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

df_hs = pd.read_csv('res_hs.txt')
df_pcre = pd.read_csv('res_pcre.txt')
df_hs['time'] = df_hs['time']/1000
df_pcre['time'] = df_pcre['time']/1000

print(df_hs.columns)

sns.lineplot(data=df_hs, x='files', y='time', marker='o', label='HyperScan')
sns.lineplot(data=df_pcre, x='files', y='time', marker='o', label='PCRE')
plt.xlabel('n Files')
plt.ylabel('Time (in seconds)')
plt.title('Lineplot -- many files (max 3mb) vs many regexes (~300)')
plt.legend()
plt.grid()
# Zapis wykresu do pliku PNG
plt.savefig('hs_300rgx_4000files.png')
