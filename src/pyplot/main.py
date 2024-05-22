# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.ticker as mtick

def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    # print_hi('PyCharm')
    d=3
    title="DTLZ1 gen 249 changed preference"
    filename="../output/test_module/run11/pop_249.txt"
    pareto = "../pf_data/dtlz/dtlz1.3D.pf"
    X,Y,Z=[],[],[]
    X1,Y1,Z1=[],[],[]
    with open(pareto,'r') as f:
        lines=f.readlines()
        for line in lines:
            value=[float(s) for s in line.split()]
            X.append(value[0])
            Y.append(value[1])
            Z.append(value[2])
    with open(filename,'r') as f:
        lines=f.readlines()
        for line in lines:
            value=[float(s) for s in line.split()]
            X1.append(value[0])
            Y1.append(value[1])
            Z1.append(value[2])
    # plot 3D
    if d==3:
        fig = plt.figure(figsize=(15, 15))
        ax = plt.axes(projection='3d')
        ax.set_xlabel("x1", fontsize=18)
        ax.set_ylabel('x2', fontsize=18)
        ax.set_zlabel('x3', fontsize=18)
        # ax.scatter(X,Y,Z)
        ax.scatter(X, Y, Z, marker='o', s=0.5, cmap='jet',zorder=1,alpha=0.3)
        ax.scatter(X1, Y1, Z1, marker='^', s=50, color='red', cmap='jet', label='preference point',zorder=2)
        # ax.set_xlim(0,1)
        # ax.set_ylim(0,1)
        # ax.set_zlim(0,1)
        plt.title(title, fontsize=20)
        plt.legend(fontsize=15)
        plt.tick_params(labelsize=15)
        plt.savefig(title + ".png")
        # plt.xticks(fontsize=15)
        # plt.yticks(fontsize=15)
        # plt.zticks(fontsize=15)
        # plt.ticks(fontsize=15)
        plt.show()

    if d==2:
        plt.figure(figsize=(15,15))
        plt.style.use('seaborn-bright')
        plt.plot_date(X,Y,linewidth=0.5,label='pareto front',alpha=0.3,zorder=1)
        plt.scatter(X1,Y1,color='r',marker='^',cmap='jet',s=80,label='preference point',zorder=2)
        plt.title(title,fontsize=20)
        plt.xlabel('x1',fontsize=18)
        plt.ylabel('y1',fontsize=18)
        plt.legend(fontsize=18)
        plt.xticks(fontsize=15)
        plt.yticks(fontsize=15)
        ax=plt.gca()
        ax.xaxis.set_major_formatter(mtick.FormatStrFormatter('%.1f'))
        plt.grid(linestyle='-.',linewidth=2)

        plt.savefig(title+'.png')
        plt.show()







# See PyCharm help at https://www.jetbrains.com/help/pycharm/
