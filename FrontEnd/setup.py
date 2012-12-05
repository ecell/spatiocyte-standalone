from distutils.core import setup, Extension
from Cython.Distutils import build_ext


sourceFileList = ["./python/FrontMain.cpp",
	"./python/FrontEndAdapter.pyx",
	]




IncludeDirList = ["./ecell3Module","./python","./python/util","./python/intermediate"]

setup(
  name = 'FrontEndAdapter',
  cmdclass = {'build_ext': build_ext},
  ext_modules = [
	Extension(
		"FrontEndAdapter",
		sourceFileList,
		language='c++',
		include_dirs=IncludeDirList,
		libraries = ['gsl', 'gslcblas'],
		library_dirs = ['/usr/lib']
		)
	]
)





