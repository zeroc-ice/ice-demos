//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import UIKit

class LoadingButton: UIButton {
    var title: String?
    private var indicator: UIActivityIndicatorView!

    @IBInspectable var activityIndicatorColor: UIColor = .white {
        didSet {
            indicator.color = activityIndicatorColor
        }
    }

    var loading: Bool = false {
        didSet {
            if loading {
                indicator = (indicator == nil) ? createActivityIndicator(color: activityIndicatorColor) : indicator
                title = titleLabel?.text
                setTitle("", for: .normal)
                startAnimating()
            } else {
                setTitle(title, for: .normal)
                indicator.stopAnimating()
            }
        }
    }

    private func centerIndicator() {
        let xCenter = NSLayoutConstraint(item: self,
                                         attribute: .centerX,
                                         relatedBy: .equal,
                                         toItem: indicator,
                                         attribute: .centerX,
                                         multiplier: 1, constant: 0)
        let yCenter = NSLayoutConstraint(item: self,
                                         attribute: .centerY,
                                         relatedBy: .equal,
                                         toItem: indicator,
                                         attribute: .centerY,
                                         multiplier: 1, constant: 0)
        addConstraint(yCenter)
        addConstraint(xCenter)
    }

    private func createActivityIndicator(color: UIColor) -> UIActivityIndicatorView {
        let indicator = UIActivityIndicatorView()
        indicator.color = color
        indicator.hidesWhenStopped = true
        return indicator
    }

    private func startAnimating() {
        indicator.translatesAutoresizingMaskIntoConstraints = false
        addSubview(indicator)
        centerIndicator()
        indicator.startAnimating()
    }
}
