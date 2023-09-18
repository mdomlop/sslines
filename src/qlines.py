#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
ZetCode PyQt5 tutorial

In this example we draw 6 lines using
different pen styles.

Author: Jan Bodnar
Website: zetcode.com
Last edited: August 2017
"""

from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.QtGui import QPainter, QPen, QColor
from PyQt5.QtCore import Qt
import sys
from random import randint
import time

class Example(QWidget):

    def __init__(self):
        super().__init__()

        self.initUI()


    def initUI(self):

        self.setGeometry(30, 30, 800, 600)
        self.setWindowTitle('Pen styles')
        self.show()


    def paintEvent(self, qp):

        qp = QPainter()
        qp.begin(self)
        
        orig = 0
        for i in range(5):
            r = randint(0, 256)
            g = randint(0, 256)
            b = randint(0, 256)
            color = QColor(r, g, b)
            
            origx = randint(0, 600)
            origy = randint(0, 600)
            destx = randint(0, 600)
            desty = randint(0, 600)
        
            pen = QPen(color, 2, Qt.SolidLine)
            qp.setPen(pen)
            qp.drawLine(origx, origy, destx, desty)
            time.sleep(0.01)
            qp.show()
        qp.end()



if __name__ == '__main__':

    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())
