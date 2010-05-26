using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HUD_Installer
{
    interface Selectable
    {
        bool IsSelected(string id);
    }
}
