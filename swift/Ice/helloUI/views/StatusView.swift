// Copyright (c) ZeroC, Inc.

import SwiftUI

struct StatusView: View {
    @Binding var isSpinning: Bool
    @Binding var text: String

    var body: some View {
        HStack(alignment: .center) {
            ProgressView().hidden(!isSpinning)
            Spacer()
            Text(text)
            Spacer()
        }.background(
            RoundedRectangle(cornerRadius: 10.0)
                .fill(Color.clear)
                .frame(height: 50)
        ).padding()
    }
}

extension View {
    func hidden(_ shouldHide: Bool) -> some View {
        opacity(shouldHide ? 0 : 1)
    }
}
