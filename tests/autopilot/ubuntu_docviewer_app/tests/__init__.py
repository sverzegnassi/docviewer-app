# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
#
# Copyright (C) 2013, 2014 Canonical Ltd.
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

import os
from autopilot.input import Mouse, Touch, Pointer
from autopilot.platform import model
from autopilot.testcase import AutopilotTestCase
import ubuntuuitoolkit

from ubuntu_docviewer_app import emulators


class DocviewerTestCase(AutopilotTestCase):

    """A common test case class that provides several useful methods for
    docviewer-app tests.

    """

    if model() == 'Desktop':
        scenarios = [('with mouse', dict(input_device_class=Mouse))]
    else:
        scenarios = [('with touch', dict(input_device_class=Touch))]

    sample_dir = "/usr/lib/python3/dist-packages/"

    def setUp(self):
        self.binary = 'ubuntu-docviewer-app'
        self.source_dir = os.path.dirname(
            os.path.dirname(os.path.abspath('.')))
        self.build_dir = self._get_build_dir()

        self.local_location = self.build_dir
        self.local_location_qml = os.path.join(self.build_dir,
                                               'src', 'app',
                                               'qml',
                                               'ubuntu_docviewer_app.qml')
        self.local_location_binary = os.path.join(self.build_dir,
                                                  'src', 'app', self.binary)
        self.installed_location_binary = os.path.join('/usr/bin/', self.binary)
        self.installed_location_qml = \
            '/usr/share/ubuntu_docviewer_app/qml/ubuntu_docviewer_app.qml'

        self.pointing_device = Pointer(self.input_device_class.create())
        super(DocviewerTestCase, self).setUp()

    def launch_test_local(self, arg):
        self.app = self.launch_test_application(
            self.local_location_binary,
            arg,
            app_type='qt',
            emulator_base=ubuntuuitoolkit.UbuntuUIToolkitCustomProxyObjectBase)

    def launch_test_installed(self, arg):
        self.app = self.launch_test_application(
            self.installed_location_binary,
            arg,
            app_type='qt',
            emulator_base=ubuntuuitoolkit.UbuntuUIToolkitCustomProxyObjectBase)

    @property
    def main_view(self):
        return self.app.select_single(emulators.MainView)

    def _get_build_dir(self):
        """
        Returns the build dir after having parsed the CMake config file
        generated by Qt Creator. If it cannot find it or it cannot be parsed,
        an in-tree build is assumed and thus returned.
        """
        try:
            cmake_config = CMakePluginParser.CMakePluginParser(os.path.join(
                self.source_dir, 'CMakeLists.txt.user'))
            build_dir = cmake_config.active_build_dir
        except:
            build_dir = self.source_dir

        return build_dir
