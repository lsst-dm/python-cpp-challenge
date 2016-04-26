from . import basics_test
from . import converters_test
from . import containers_test

import unittest

containers_test.DoodadSetTestCase.test_downcast = \
    unittest.skip("extensions module not implemented")(containers_test.DoodadSetTestCase.test_downcast)