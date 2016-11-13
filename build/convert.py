import glob

from PIL import Image


files = glob.glob('*.bmp')

for f in files:
	print('Convert Image: ' + f)
	img = Image.open(f)

	img.save(f[0:len(f)-3] + 'png')
