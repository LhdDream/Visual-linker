import matplotlib.pyplot as plt
import numpy as np
col=[]
for i in range(1,8):
    col.append("Day"+str(i))
row=[]
for i in range(1,13):
    row.append(i)
vals=np.random.rand(12,7)

plt.figure(figsize=(20,8))
tab = plt.table(cellText=vals,
              colLabels=col,
             rowLabels=row,
              loc='center',
              cellLoc='center',
              rowLoc='center')
tab.scale(1,2)
plt.axis('off')
