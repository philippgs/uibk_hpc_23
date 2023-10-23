import matplotlib.pyplot as plt

labels = [
    1056,
    2112,
    4224,
]

r12 = [0.478, 1.002, 2.564]

r24 = [1.094, 2.066, 4.073]

r48 = [1.104, 2.133, 4.165]

r96 = [1.079, 2.107, 4.166]

x = ["12", "24", "48", "96"]

for label, data in zip(labels, zip(r12, r24, r48, r96)):
    plt.plot(x, data, label=label)

# naming the x axis
plt.xlabel("Number of Cores")
# naming the y axis
plt.ylabel("Execution Time in Seconds")
plt.legend()
plt.savefig("1D-nonBlocking.png")

plt.close()
#### Blocking

b12 = [0.891, 1.853, 4.279]
b24 = [2.226, 4.456, 9.214]
b48 = [2.317, 4.517, 9.082]
b96 = [2.511, 4.780, 9.391]


for label, data in zip(labels, zip(b12, b24, b48, b96)):
    plt.plot(x, data, label=label)

# naming the x axis
plt.xlabel("Number of Cores")
# naming the y axis
plt.ylabel("Execution Time in Seconds")
plt.legend()
plt.savefig("1D-Blocking.png")
plt.close()

#### Non-blocking
x = ["24", "48", "96"]

labels = [128, 256, 512]

r24 = [0.184, 0.536, 3.616]

r48 = [0.197, 0.369, 1.663]

r96 = [0.540, 0.673, 1.382]

for label, data in zip(labels, zip(r24, r48, r96)):
    plt.plot(x, data, label=label)

# naming the x axis
plt.xlabel("Number of Cores")
# naming the y axis
plt.ylabel("Execution Time in Seconds")
plt.legend()
plt.savefig("2D-nonBlocking.png")

plt.close()
#### Blocking

b24 = [0.252, 0.709, 3.737]

b48 = [0.328, 0.644, 2.299]

b96 = [0.795, 1.072, 2.470]

for label, data in zip(labels, zip(b24, b48, b96)):
    plt.plot(x, data, label=label)

# naming the x axis
plt.xlabel("Number of Cores")
# naming the y axis
plt.ylabel("Execution Time in Seconds")
plt.legend()
plt.savefig("2D-Blocking.png")

plt.close()
