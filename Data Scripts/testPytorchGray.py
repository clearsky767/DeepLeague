#-*- coding: UTF-8 -*- 
from PIL import Image
import numpy as np
import torch
#import torchvision.transforms
import matplotlib.pyplot as plt

def to_gray(tensor):
    R = tensor[0]
    G = tensor[1]
    B = tensor[2]
    img_tensor=0.299*R+0.587*G+0.114*B
    #img_tensor = img_tensor.view(1,tensor.shape[1],tensor.shape[2]) 
    return img_tensor

def to_grad_x(tensor):
    h = tensor.shape[0]
    w = tensor.shape[1]
    img_tensor_x = tensor[0:h-1,:] - tensor[1:h,:]
    print(img_tensor_x.shape)
    return torch.abs(img_tensor_x)

def to_grad_y(tensor):
    h = tensor.shape[0]
    w = tensor.shape[1]
    img_tensor_y = tensor[:,0:w-1] - tensor[:,1:w]
    print(img_tensor_y.shape)
    return torch.abs(img_tensor_y)

def get_image(path):
    #获取图片
    img = Image.open(path)
    img = img.convert('RGB')
    img_tensor = torch.ByteTensor(torch.ByteStorage.from_buffer(img.tobytes()))
    img_tensor = img_tensor.view(img.size[1],img.size[0],3)
    img_tensor = img_tensor.transpose(0, 1).transpose(0, 2).contiguous()
    img_tensor = img_tensor.float().div(255)
    img_gray = to_gray(img_tensor)
    imt_grad = to_grad_y(img_gray)
    return imt_grad

#to_pil_image = transforms.ToPILImage()

def show_image(img_tensor):
    # 方法1：Image.show() 
    # # transforms.ToPILImage()中有一句 
    # # npimg = np.transpose(pic.numpy(), (1, 2, 0)) 
    # # 因此pic只能是3-D Tensor，所以要用image[0]消去batch那一维 
    # img = to_pil_image(image[0]) 
    # img.show() 
    # # 方法2：plt.imshow(ndarray) img = image[0] 
    # # plt.imshow()只能接受3-D Tensor，所以也要用image[0]消去batch那一维 
    img = img_tensor.numpy()
    # FloatTensor转为ndarray 
    #img = np.transpose(img, (1,2,0)) 
    # 把channel那一维放到最后 
    # 显示图片 
    plt.imshow(img,cmap="gray") 
    plt.show()

img = get_image("chongzi.jpeg")
print(type(img))
print(img.shape)
print(img)
print(img.size())
show_image(img)