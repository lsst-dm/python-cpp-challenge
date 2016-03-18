#!/usr/bin/env python

import unittest

import challenge.basics
import challenge.extensions

class ThingamajigTestMixin(object):
    """Base class for all Thingamajig TestCases.

    Derived classes must define a setUp() method that defines a self.extra
    attribute that matches the template type of the Thingamajig being tested.
    """

    def test_standard_ctor_pos_all(self):
        """Test standard constructor with all positional arguments.
        """
        t = challenge.extensions.Thingamajig(self.extra, "b", 0, dtype=complex)
        self.assertIsInstance(t, challenge.extensions.Thingamajig)
        self.assertEqual(t.get_extra(), self.extra)
        self.assertEqual(t.name, "b")
        self.assertEqual(t.value, 0)

    def test_standard_ctor_pos_default(self):
        """Test standard constructor with positional arguments, with a default.
        """
        t = challenge.extensions.Thingamajig(self.extra, "b")
        self.assertIsInstance(t, challenge.extensions.Thingamajig)
        self.assertEqual(t.get_extra(), self.extra)
        self.assertEqual(t.name, "b")
        self.assertEqual(t.value, 1)

    def test_standard_ctor_kwarg_al(self):
        """Test standard constructor with all keyword arguments.
        """
        t = challenge.extensions.Thingamajig(name="c", value=2,
                                             extra=self.extra)
        self.assertIsInstance(t, challenge.extensions.Thingamajig)
        self.assertEqual(t.get_extra(), self.extra)
        self.assertEqual(t.name, "c")
        self.assertEqual(t.value, 2)

    def test_standard_ctor_kwarg_default(self):
        """Test standard constructor with keyword arguments, with a default.
        """
        t = challenge.extensions.Thingamajig(name="d", extra=self.extra)
        self.assertIsInstance(t, challenge.extensions.Thingamajig)
        self.assertEqual(t.get_extra(), self.extra)
        self.assertEqual(t.name, "d")
        self.assertEqual(t.value, 1)

    def test_standard_ctor_mixed(self):
        """Test standard constructor with a mix of positional and keyword args.
        """
        t = challenge.extensions.Thingamajig(self.extra, name="e", value=3)
        self.assertIsInstance(t, challenge.extensions.Thingamajig)
        self.assertEqual(t.get_extra(), self.extra)
        self.assertEqual(t.name, "e")
        self.assertEqual(t.value, 3)

    def test_clone(self):
        """Test calling the clone() method, including downcasting.
        """
        t1 = challenge.extensions.Thingamajig(self.extra, "g", 5)
        t2 = t1.clone()
        self.assertIsInstance(t2, challenge.extensions.Thingamajig)
        self.assertEqual(t1.name, t2.name)
        self.assertEqual(t1.value, t2.value)
        self.assertEqual(t1.get_extra(), t2.get_extra())
        self.assertTrue(
            challenge.basics.adjacent(t1.get_secret(), t2.get_secret())
        )


class ComplexThingamajigTestCase(ThingamajigTestMixin, unittest.TestCase):
    """Test case for for Thingamajig<std::complex<float>> bindings.

    All tests also require Doodad to be fully wrapped, and most require the
    standard constructor to be wrapped to support at least positional arguments.
    """

    def setUp(self):
        self.extra = -4j

    def test_types(self):
        """Test the inheritance relationships and dtype status of Thingamajig.
        """
        t = challenge.extensions.Thingamajig(-1j, "a", 0, dtype=complex)
        self.assertIsInstance(t, challenge.basics.Doodad)
        self.assertIsInstance(t, challenge.extensions.Thingamajig)
        # Since "numpy.dtype(complex) == complex", we're not requiring
        # the more restrictive "t.dtype is complex".
        self.assertEqual(t.dtype, complex)


class DoodadThingamajigTestCase(ThingamajigTestMixin, unittest.TestCase):
    """Test case for for Thingamajig<Doodad> bindings.

    All tests also require Doodad to be fully wrapped, and most require the
    standard constructor to be wrapped to support at least positional arguments.
    """

    def compare_Doodads(self, a, b, msg=None):
        if a.name != b.name or a.value != b.value:
            if msg is not None:
                template = "({a.name}, {a.value}) != ({b.name}, {b.value})"
                msg = template.format(a=a, b=b)
            self.failureException(msg)

    def __init__(self, *args, **kwds):
        super(DoodadThingamajigTestCase, self).__init__(*args, **kwds)
        self.addTypeEqualityFunc(challenge.basics.Doodad, self.compare_Doodads)

    def setUp(self):
        self.extra = challenge.basics.Doodad("asdf", 120)

    def test_types(self):
        """Test the inheritance relationships and dtype status of Thingamajig.
        """
        t = challenge.extensions.Thingamajig(
            self.extra, "a", 0,
            dtype=challenge.basics.Doodad
        )
        self.assertIsInstance(t, challenge.basics.Doodad)
        self.assertIsInstance(t, challenge.extensions.Thingamajig)
        self.assertEqual(t.dtype, challenge.basics.Doodad)


if __name__ == "__main__":
    unittest.main()
