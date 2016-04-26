from . import basics_test
from . import converters_test
from . import containers_test

import unittest

basics_test.DoodadTestCase.test_const = \
    unittest.skip("const correctness not supported")(
        basics_test.DoodadTestCase.test_const
    )

containers_test.DoodadSetTestCase.test_downcast = \
    unittest.skip("extensions module not implemented")(
        containers_test.DoodadSetTestCase.test_downcast
    )

converters_test.SwigTestCase.test_const_guarantees = \
    unittest.skip("const correctness not supported")(
        converters_test.SwigTestCase.test_const_guarantees
    )
