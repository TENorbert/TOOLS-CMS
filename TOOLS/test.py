# Copyright 2012 the rootpy developers
# distributed under the terms of the GNU General Public License
import ROOT
from rootpy.plotting import Hist
from rootpy.plotting.style import get_style
from rootpy.plotting.style.cmstdr import labels


def test_cmstdr():

    style = get_style('CMSTDR')
    with style:
        hpx = Hist(100, -4, 4, name="hpx", title="This is the px distribution")
        ROOT.gRandom.SetSeed()
        for i in xrange(1000):
            hpx.Fill(ROOT.gRandom.Gaus())
        hpx.GetXaxis().SetTitle("random variable [unit]")
        hpx.GetYaxis().SetTitle("#frac{dN}{dr} [unit^{-1}]")
        hpx.SetMaximum(100.)
        hpx.Draw()
        labels.CMS_label("Testing 2050", sqrts=100)

if __name__ == "__main__":
    import nose
    nose.runmodule()
