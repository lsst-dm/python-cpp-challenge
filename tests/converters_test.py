#!/usr/bin/env python

import unittest
import challenge.basics
import challenge.converters

class SwigTestCase(unittest.TestCase):
    """Test case for for Swig typemaps for Doodads.
    """

    def test_shared_ptr_return(self):
        """Test that a Swig-built module can return a "shared_ptr<Doodad>".
        """
        r = challenge.converters.make_sptr("a", 5)
        self.assertIsInstance(r, challenge.basics.Doodad)
        self.assertEqual(r.name, "a")
        self.assertEqual(r.value, 5)

    def test_const_shared_ptr_return(self):
        """Test that a Swig-built module can return a "shared_ptr<Doodad const>".
        """
        r = challenge.converters.make_csptr("a", 5)
        self.assertIsInstance(r, challenge.basics.Doodad)
        self.assertEqual(r.name, "a")
        self.assertEqual(r.value, 5)

    def test_pass_reference(self):
        """Test that a Swig-built module can accept "Doodad &".
        """
        d = challenge.basics.MutableDoodad("b", 6)
        self.assertTrue(challenge.converters.accept_ref(d, "b", 6))

    def test_pass_const_reference(self):
        """Test that a Swig-built module can accept "Doodad const &".
        """
        d = challenge.basics.MutableDoodad("c", 7)
        self.assertTrue(challenge.converters.accept_cref(d, "c", 7))

    def test_pass_shared_ptr(self):
        """Test that a Swig-built module can accept "shared_ptr<Doodad>".
        """
        d = challenge.basics.MutableDoodad("d", 8)
        self.assertTrue(challenge.converters.accept_sptr(d, "d", 8))

    def test_pass_const_reference(self):
        """Test that a Swig-built module can accept "shared_ptr<Doodad const>".
        """
        d = challenge.basics.ImmutableDoodad("e", 9)
        self.assertTrue(challenge.converters.accept_csptr(d, "e", 9))

    def test_const_guarantees(self):
        """Test that C++ constness restrictions are propagated to Python."""
        r = challenge.converters.make_csptr("a", 5)
        self.assertRaises(TypeError, r, setattr, "name", "f")
        self.assertRaises(TypeError, r, setattr, "value", "100")
        self.assertTrue(challenge.converters.accept_cref(r, "a", 5))
        self.assertTrue(challenge.converters.accept_csptr(r, "a", 5))
        self.assertRaises(TypeError, challenge.converters.accept_sptr,
                          r, "a", 5)
        self.assertRaises(TypeError, challenge.converters.accept_ref,
                          r, "a", 5)


if __name__ == "__main__":
    unittest.main()
