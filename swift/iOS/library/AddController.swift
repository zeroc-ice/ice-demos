//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import UIKit

class AddController: DetailController {
    var library: LibraryPrx!

    override func viewDidLoad() {
        title = "New Book"
        tableView.allowsSelectionDuringEditing = true
        setEditing(true, animated: false)
    }

    override func viewWillAppear(_ animated: Bool) {
        super.viewWillAppear(animated)
        navigationItem.leftBarButtonItem?.isEnabled = true
        navigationItem.rightBarButtonItem?.isEnabled = !book.isbn.isEmpty
        setEditing(true, animated: false)
        navigationItem.setHidesBackButton(false, animated: animated)
    }

    @IBAction func save(sender _: Any?) {
        saving(true)

        library.createBookAsync(isbn: book.isbn, title: book.title, authors: book.authors).done { _ in
            self.saving(false)
            self.navigationController?.popViewController(animated: true)
        }.catch { error in
            if error is BookExistsException {
                self.saving(false)
                let alert = UIAlertController(title: "Error", message: "That ISBN number already exists",
                                              preferredStyle: .alert)
                alert.addAction(UIAlertAction(title: "OK", style: .default, handler: nil))
                self.present(alert, animated: true, completion: nil)
            } else {
                self.exception(error)
            }
        }
    }

    func startEdit(book: BookDescription, library: LibraryPrx?) {
        self.book = book
        self.library = library
        setEditing(true, animated: false)
    }
}
