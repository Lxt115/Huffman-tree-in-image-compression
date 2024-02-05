> 本项目实现了使用二叉树与哈夫曼编码进行图片压缩的程序。它可以读取一幅 BMP 格式的图片文件，统计 256 种不同字节的重复次数，并以每种字节重复次数作为权值构造一颗有 256 个叶子节点的哈夫曼二叉树。然后，利用生成的哈夫曼树产生的哈夫曼编码对图片文件进行压缩。压缩后的文件与原图片文件同名，加上后缀.huf（保留原后缀），例如 pic.bmp 压缩后 pic.bmp.huf。
> This project implements a program that uses binary trees and Huffman coding for image compression. It can read a BMP image file, count the occurrences of 256 different bytes, and construct a Huffman binary tree with 256 leaf nodes, using the repetition count of each byte as its weight. It then compresses the image file using the generated Huffman tree and Huffman coding. The compressed file has the same name as the original image file, with the addition of the .huf extension, for example, pic.bmp is compressed as pic.bmp.huf.

# 实现流程
以下是实现二叉树与哈夫曼图片压缩软件的流程：

读取图片文件并统计权值：程序首先读取指定的图片文件，然后统计该文件中 256 种不同字节的重复次数，以每种字节重复次数作为权值。

生成 Huffman 树：根据统计得到的权值，构造一颗有 256 个叶子节点的哈夫曼二叉树。哈夫曼树的构建过程中，使用最小堆或其他适合的数据结构来管理节点。

生成 Huffman 编码：利用生成的哈夫曼树，为每个字节生成对应的哈夫曼编码。哈夫曼编码可以通过遍历哈夫曼树的路径来获得。

压缩图片文件：使用生成的哈夫曼编码，将原始图片文件中的每个字节替换为对应的哈夫曼编码。将替换后的字节序列写入一个新的文件中，作为压缩后的文件。

保存压缩的文件：将压缩后的文件保存到磁盘上，文件名与原始图片文件相同，只是加上了.huf 后缀。


# Implementation
The following is the workflow for implementing the Binary Tree and Huffman Image Compression software:

Read the image file and count weights: The program first reads the specified image file and counts the number of occurrences of each of the 256 different bytes, using the repetition count as the weight.

Generate a Huffman tree: Based on the computed weights, construct a Huffman binary tree with 256 leaf nodes. During the construction of the Huffman tree, use a minimum heap or any other suitable data structure to manage the nodes.

Generate Huffman codes: Utilize the generated Huffman tree to generate Huffman codes for each byte. Huffman codes can be obtained by traversing the paths of the Huffman tree.

Compress the image file: Using the generated Huffman codes, replace each byte in the original image file with its corresponding Huffman code. Write the resulting byte sequence to a new file as the compressed file.

Save the compressed file: Save the compressed file to the disk with the same name as the original image file, but with the addition of the .huf extension.
