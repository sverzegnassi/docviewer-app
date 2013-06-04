# -*- Mode: Python; coding: utf-8; indent-tabs-mode: nil; tab-width: 4 -*-
# Copyright 2013 Canonical
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3, as published
# by the Free Software Foundation.

"""Docviewer app autopilot tests."""

import os.path

from autopilot.input import Mouse, Touch, Pointer
from autopilot.platform import model
from autopilot.testcase import AutopilotTestCase

from ubuntu_docviewer_app.emulators.main_window import MainWindow


class DocviewerTestCase(AutopilotTestCase):

    """A common test case class that provides several useful methods for
    docviewer-app tests.

    """
    if model() == 'Desktop':
        scenarios = [('with mouse', dict(input_device_class=Mouse))]
    else:
        scenarios = [('with touch', dict(input_device_class=Touch))]

    local_location = "../../ubuntu-docviewer-app.qml"

    def setUp(self):
        self.pointing_device = Pointer(self.input_device_class.create())
        super(DocviewerTestCase, self).setUp()
        if os.path.exists(self.local_location):
            self.launch_test_local()
        else:
            self.launch_test_installed()

    def launch_test_local(self):
        self.app = self.launch_test_application(
            "qmlscene",
            self.local_location,
            app_type='qt')

    def launch_test_installed(self):
        self.app = self.launch_test_application(
            "qmlscene",
            "/usr/share/ubuntu-docviewer-app/ubuntu-docviewer-app.qml",
            "--desktop_file_hint=/usr/share/applications/ubuntu-docviewer-app.desktop",
            app_type='qt')

    @property
    def main_window(self):
        return MainWindow(self.app)
