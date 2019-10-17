//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import PromiseKit
import UIKit

protocol DetailControllerDelegate: AnyObject {
    func bookUpdated(_ book: BookDescription)
    func bookDeleted()
    func destroySession()
}

class DetailController: UIViewController {
    var book: BookDescription!
    var updated: BookDescription!

    @IBOutlet var tableView: UITableView!

    var selectedIndexPath: IndexPath!

    weak var delegate: DetailControllerDelegate!
    var saving: Bool = false
    var changed: Bool = false

    override func viewDidLoad() {
        navigationItem.rightBarButtonItem = editButtonItem
    }

    override func viewWillAppear(_: Bool) {
        //
        // Remove any existing selection.
        //
        if let selectedIndexPath = self.selectedIndexPath {
            tableView.deselectRow(at: selectedIndexPath, animated: false)
        }

        tableView.setEditing(isEditing, animated: false)

        //
        // Redisplay the data.
        //
        tableView.reloadData()
    }

    override func viewWillDisappear(_: Bool) {
        if isMovingToParent, changed {
            delegate!.bookUpdated(book!)
        }
    }

    override func setEditing(_ editing: Bool, animated: Bool) {
        super.setEditing(editing, animated: animated)
        navigationItem.setHidesBackButton(editing, animated: animated)

        if let tableView = self.tableView {
            tableView.setEditing(editing, animated: animated)
            tableView.reloadData()
        }
    }

    @IBAction func removeBook(sender _: AnyObject?) {
        navigationController?.popViewController(animated: true)
        delegate.bookDeleted()
    }

    func startEdit(_ book: BookDescription) {
        changed = false
        self.book = book
        setEditing(false, animated: false)
    }

    func saving(_ status: Bool) {
        saving = status
        tableView.reloadData()
        if saving {
            tableView.alpha = 0.5
        } else {
            tableView.alpha = 1.0
        }
    }

    func commitEdit() {
        book = updated
        updated = nil
        changed = true
        saving(false)
    }

    func saveIsbn(isbn: String) {
        precondition(book.proxy == nil)
        updated = book
        updated.isbn = isbn
        commitEdit()
    }

    func saveTitle(title: String) {
        saving(true)
        updated = book
        updated.title = title

        if let proxy = book?.proxy {
            proxy.setTitleAsync(title).done {
                self.commitEdit()
            }.catch { error in
                self.exception(error)
            }
        } else {
            commitEdit()
        }
    }

    func saveAuthors(author: String) {
        saving(true)
        let index = selectedIndexPath.row

        updated = book

        if index == updated.authors.count {
            updated.authors.append(author)
        } else {
            updated.authors[index] = author
        }

        if let proxy = book.proxy {
            proxy.setAuthorsAsync(updated.authors).done {
                self.commitEdit()
            }.catch { error in
                self.exception(error)
            }
        } else {
            commitEdit()
        }
    }

    func rentBook(value: String) {
        saving(true)
        updated = book
        updated.rentedBy = value

        precondition(book.proxy != nil)

        book.proxy!.rentBookAsync(value).done {
            self.commitEdit()
        }.catch { error in
            self.exception(error)
        }
    }

    override func prepare(for segue: UIStoryboardSegue, sender _: Any?) {
        if segue.identifier == "ShowEdit" {
            let ec = segue.destination as! EditController
            var name: String!
            var value: String!

            switch selectedIndexPath.section {
            case 0:
                name = "ISBN"
                value = book.isbn
            case 1:
                name = "Title"
                value = book.title
            case 2:
                let author = (selectedIndexPath.row >= book.authors.count) ? "" : book.authors[selectedIndexPath.row]
                name = "Author"
                value = author
            case 3:
                name = "Enter Book Renter Name"
                value = ""
            default:
                fatalError()
            }
            ec.startEdit(name, value: value)
        }
    }

    @IBAction func unwindFromEdit(segue: UIStoryboardSegue) {
        if segue.source is EditController {
            let ec = segue.source as! EditController
            let value = ec.textField.text!

            switch selectedIndexPath.section {
            case 0:
                saveIsbn(isbn: value)
            case 1:
                saveTitle(title: value)
            case 2:
                saveAuthors(author: value)
            case 3:
                rentBook(value: value)
            default:
                fatalError()
            }
        }
    }

    func exception(_ error: Error) {
        saving(false)
        // Discard the edit.
        updated = nil

        if let indexPath = selectedIndexPath {
            tableView.deselectRow(at: indexPath, animated: false)
        }

        var s: String!
        var fatal = false

        // Ignore ObjectNotExistExceptiojn
        if error is Ice.ObjectNotExistException {
            navigationController?.popViewController(animated: true)
            delegate.bookDeleted()
            s = "The current book has been removed"
        } else if let ex = error as? BookRentedException {
            s = "The book has already been rented"
            book.rentedBy = ex.renter
            tableView.reloadData()
        } else if error is BookNotRentedException {
            s = "The book has already been returned."
            book.rentedBy = ""
            tableView.reloadData()
        } else {
            s = error.localizedDescription

            // BUGFIX: In the event of a fatal exception we want to pop back to the login view.
            // However, doing so directly by calling [self.navigationController popToRootViewControllerAnimated:YES];
            // causes the navigation view & the bar to get out of sync. So instead, we pop to the root view
            // in the alert view didDismissWithButtonIndex callback.
            fatal = true
        }

        //
        // open an alert with just an OK button
        //
        let alert = UIAlertController(title: "Error", message: s, preferredStyle: .alert)
        alert.addAction(UIAlertAction(title: "OK", style: .default) { _ in
            if fatal {
                self.delegate.destroySession()
                self.navigationController?.popToRootViewController(animated: true)
            }
        })
        present(alert, animated: true, completion: nil)
    }
}

extension DetailController: UITableViewDelegate, UITableViewDataSource {
    func numberOfSections(in _: UITableView) -> Int {
        //
        // If we're in "add book mode" then the rented-by section is not visible.
        // 4 sections. ISBN, Title, Authors, Rented By | Remove books.
        //
        return book.proxy == nil ? 3 : 4
    }

    func tableView(_: UITableView, numberOfRowsInSection section: Int) -> Int {
        var nrows: Int = 1
        if section == 2 { // Authors
            nrows = isEditing ? book.authors.count + 1 : book.authors.count
        }
        return nrows
    }

    func tableView(_: UITableView,
                   editingStyleForRowAt indexPath: IndexPath) -> UITableViewCell.EditingStyle {
        if !isEditing || saving || indexPath.section != 2 {
            return .none
        }

        return indexPath.row == book.authors.count ? .insert : .delete
    }

    func tableView(_ tableView: UITableView,
                   commit editingStyle: UITableViewCell.EditingStyle,
                   forRowAt indexPath: IndexPath) {
        //
        // If row is deleted, remove it from the list.
        //
        if editingStyle == .delete {
            updated = book
            saving(true)
            //
            // Remove the author from the book.
            //
            updated.authors.remove(at: indexPath.row)
            book.authors = updated.authors
            if let proxy = book.proxy {
                proxy.setAuthorsAsync(updated.authors).done {
                    self.commitEdit()
                }.catch { error in
                    self.exception(error)
                }
            } else {
                commitEdit()
            }

            //
            // Animate the deletion from the table.
            //
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            selectedIndexPath = indexPath
            performSegue(withIdentifier: "ShowEdit", sender: self)
        }
    }

    func tableView(_ tableView: UITableView,
                   heightForHeaderInSection section: Int) -> CGFloat {
        if isEditing, section == 3 {
            return 0 // Remove button
        } else {
            return tableView.sectionHeaderHeight
        }
    }

    func tableView(_: UITableView,
                   heightForRowAt indexPath: IndexPath) -> CGFloat {
        if indexPath.section == 1 {
            if book.title.isEmpty {
                return 44
            }

            //
            // The width of the table is 320 - 20px of left & right padding. We don't want to let the title
            // go past 200px.
            //
            let rect = book.title.boundingRect(
                with: isEditing ? CGSize(width: 250, height: 200) :
                    CGSize(width: 260, height: 200),
                options: [.usesLineFragmentOrigin, .usesFontLeading],
                attributes: [NSAttributedString.Key.font: UIFont.boldSystemFont(ofSize: 20)],
                context: nil
            )
            return rect.size.height + 20 // 20px padding.
        } else {
            return 44 // 20px padding + 22.f for the 20pt font.
        }
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        //
        // 4 Possible sections. Section 1 and 4 are special. The title section, and the remove book section.
        //
        if indexPath.section == 1 { // Title
            var cell = tableView.dequeueReusableCell(withIdentifier: "TitleCell")
            if cell == nil {
                //
                // Create a new cell.
                //
                cell = UITableViewCell(style: .default, reuseIdentifier: "TitleCell")

                //
                // Add a label to the frame
                //
                let textView = UILabel(frame: CGRect.zero)
                cell!.contentView.addSubview(textView)
                textView.numberOfLines = 0
                textView.textColor = UIColor.black
                textView.font = UIFont.boldSystemFont(ofSize: 18)
            }

            let textView = cell!.contentView.subviews[0] as! UILabel
            let rect = book.title.boundingRect(
                with: isEditing ? CGSize(width: 250, height: 200) :
                    CGSize(width: 260, height: 200),
                options: [.usesLineFragmentOrigin, .usesFontLeading],
                attributes: [NSAttributedString.Key.font: UIFont.boldSystemFont(ofSize: 18)],
                context: nil
            )
            textView.frame = CGRect(x: 10, y: 10, width: rect.size.width, height: rect.size.height + 10)
            textView.text = book.title
            return cell!
        }

        if indexPath.section == 3, isEditing { // Remove book section OR rented by section
            // Remove book section.
            var cell: UITableViewCell! = tableView.dequeueReusableCell(withIdentifier: "RemoveBook")
            if cell == nil {
                //
                // Create a new cell.
                //
                cell = UITableViewCell(style: .default, reuseIdentifier: "RemoveBook")

                //
                // Add a label to the frame,
                //
                let button = UIButton(type: .system)
                button.tag = 100
                button.frame = cell.frame

                button.setTitle("Remove Book", for: .normal)
                button.setTitleColor(UIColor.white, for: .normal)
                button.backgroundColor = UIColor.red

                button.addTarget(self, action: #selector(removeBook), for: .touchUpInside)

                button.isEnabled = !saving
                cell.contentView.addSubview(button)
            } else {
                let button = cell.contentView.viewWithTag(100) as! UIButton
                button.isEnabled = !saving
            }
            return cell!
        }

        var cell: UITableViewCell! = tableView.dequeueReusableCell(withIdentifier: "MyIdentifier")
        if cell == nil {
            cell = UITableViewCell(style: .default, reuseIdentifier: "MyIdentier")
        }

        if indexPath.section == 0 {
            cell.textLabel!.text = book.isbn
        } else if indexPath.section == 2 {
            if indexPath.row == book.authors.count {
                precondition(isEditing)
                cell.textLabel!.text = "Add new author"
            } else {
                cell.textLabel!.text = book.authors[indexPath.row]
            }
        } else if indexPath.section == 3 {
            if book.rentedBy.isEmpty {
                cell.textLabel!.text = "Not rented. Click to rent."
            } else {
                cell.textLabel!.text = book.rentedBy
            }
        }
        return cell!
    }

    func tableView(_: UITableView, titleForHeaderInSection section: Int) -> String? {
        var title: String?
        // Return the displayed title for the specified section.
        switch section {
        case 0:
            title = "ISBN"
        case 1:
            title = "Title"
        case 2:
            title = "Authors"
        case 3:
            title = isEditing ? "" : "Rented By"
        default:
            fatalError()
        }
        return title
    }

    func tableView(_: UITableView, willSelectRowAt indexPath: IndexPath) -> IndexPath? {
        if indexPath.section == 3 {
            return indexPath
        }
        //
        // Only allow selection if editing.
        //
        return isEditing ? indexPath : nil
    }

    func tableView(_: UITableView, didSelectRowAt indexPath: IndexPath) {
        //
        // Do not accept edit commands while saving is in progress.
        //
        if saving {
            return
        }
        switch indexPath.section {
        case 0:
            //
            // Its not possible to change the ISBN number of an existing book.
            //
            if book.proxy != nil {
                return
            }
        case 3:
            //
            // Its not possible to set the rented by field of a new book.
            //
            precondition(book.proxy != nil)

            if !book.rentedBy.isEmpty {
                //
                // Return the book.
                //
                let alert = UIAlertController(title: "Return Book", message: nil, preferredStyle: .actionSheet)
                alert.addAction(UIAlertAction(title: "Cancel", style: .default) { _ in
                    self.tableView.deselectRow(at: self.selectedIndexPath, animated: false)
                })

                alert.addAction(UIAlertAction(title: "Return", style: .default) { _ in
                    self.tableView.deselectRow(at: self.selectedIndexPath, animated: false)
                    self.saving(true)
                    self.book.proxy!.returnBookAsync().done {
                        self.saving(false)
                        self.tableView.deselectRow(at: self.selectedIndexPath, animated: false)
                        self.book.rentedBy = ""
                        self.tableView.reloadData()
                    }.catch { error in
                        self.exception(error)
                    }
                })

                present(alert, animated: true, completion: nil)
                selectedIndexPath = indexPath
                return
            }
        default:
            break
        }

        selectedIndexPath = indexPath
        performSegue(withIdentifier: "ShowEdit", sender: self)
    }
}
