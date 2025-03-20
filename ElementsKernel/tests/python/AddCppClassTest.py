import unittest

from ElementsKernel.AddCppClass import getClassName

class AddCppClassTestCase(unittest.TestCase):
    def testGetClassName(self):

        class_subdir = "subdir/toto/MyClass"

        subdir, class_name = getClassName(class_subdir)

        self.assertEqual(subdir, "subdir/toto")  # add assertion here
        self.assertEqual(class_name, "MyClass")

if __name__ == '__main__':
    unittest.main()
