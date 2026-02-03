import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

df_hs = pd.read_csv('res_hs.txt')
df_pcre = pd.read_csv('res_pcre.txt')

print(df_hs.columns)

sns.lineplot(data=df_hs, x='files', y='time', marker='o', c='green', label='HS')
sns.lineplot(data=df_pcre, x='files', y='time', marker='o', c='red', label='PCRE')
plt.xlabel('n Files')
plt.ylabel('Time (in milisecounds)')
plt.title('Lineplot -- many files (max 3mb) vs many regexes (~300)')
plt.legend()
plt.grid()
# Zapis wykresu do pliku PNG
plt.savefig('wykres_hs.png')
