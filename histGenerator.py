
# coding: utf-8

# In[1]:

import numpy as np
from matplotlib import pyplot as plt


# In[4]:

blA3D = np.genfromtxt('blockAverage_3D_spatial_network.out', delimiter='\n')
blAI = np.genfromtxt('blockAverage_iris.out', delimiter='\n')
blAY = np.genfromtxt('blockAverage_yeast_clean.out', delimiter='\n')
firstBlock3D = np.genfromtxt('firstBlock_3D_spatial_network.out', delimiter='\n')
firstBlockI = np.genfromtxt('firstBlock_iris.out', delimiter='\n')
fistBlockY = np.genfromtxt('firstBlock_yeast_clean.out', delimiter='\n')
randInit3D = np.genfromtxt('randInit_3D_spatial_network.out', delimiter='\n')
randInitI = np.genfromtxt('randInit_iris.out', delimiter='\n')
randInitY = np.genfromtxt('randInit_yeast_clean.out', delimiter='\n')


# In[17]:

plt.show()
plt.hist(blA3D, log=True)
plt.savefig('blA3D.png')
plt.show()
plt.hist(blAI, log=True)
plt.savefig('blAI.png')
plt.show()

plt.hist(blAY, log=True)
plt.savefig('blAY.png')
plt.show()

plt.hist(firstBlock3D, log=True)
plt.savefig('firstBlock3D.png')
plt.show()

plt.hist(firstBlockI, log=True)
plt.savefig('firstBlockI.png')
plt.show()

plt.hist(fistBlockY, log=True)
plt.savefig('firstBlockY.png')
plt.show()

plt.hist(randInit3D, log=True)
plt.savefig('randInit3D.png')
plt.show()

plt.hist(randInitI, log=True)
plt.savefig('randInitI.png')
plt.show()

plt.hist(randInitY, log=True)
plt.savefig('randInitY.png')
plt.show()


# In[ ]:



