using FlaUI.Core.AutomationElements;
using FlaUI.UIA3;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using static UiAutomate.NativeMethods;

namespace UiAutomate
{
    public struct Rect
    {
        public int width;
        public int height;
    }
    public class Program
    {
        [System.Runtime.InteropServices.DllImport("user32.dll")]
        public static extern int GetSystemMetrics(int nIndex);
        static Rect GetScreenRect()
        {
            int width = GetSystemMetrics(0);
            int height = GetSystemMetrics(1);
            var rect = new Rect
            {
                width = width,
                height = height
            };


            return rect;
        }

        static void Main(string[] args)
        {

            var app = GetApplication();
            using (var automation = new UIA3Automation())
            {
                var window = app.GetMainWindow(automation);
                var rect = GetScreenRect();
                if (rect.width > 0)
                {
                    if (!window.IsOffscreen)
                    {
                        try
                        {
                            window.Move(rect.width + 2, rect.height + 2);
                        }
                        catch { }
                    }
                }

                while (IsWindowLoading(window))
                {
                    Thread.Sleep(1000);
                }
                if (!IsLoggedIn(window))
                {
                    Button termsButton = null;
                    Button login = null;

                    Thread.Sleep(500);
                    for (int i = 0; i < 5; i++)
                    {
                        termsButton = FindAgreeToTermsButton(window);

                        if (termsButton != null)
                        {
                            break;
                        }
                        Thread.Sleep(1000);
                    }
                    if (termsButton != null)
                    {
                        termsButton.Click();
                    }
                    for (int i = 0; i < 5; i++)
                    {
                        login = FindLoginButton(window);

                        if (login != null)
                        {
                            break;
                        }
                        Thread.Sleep(1000);
                    }
                    if (login != null)
                    {
                        login.Click();
                        Thread.Sleep(1000);
                        var dialogs = window.ModalWindows;
                        if (dialogs != null && dialogs.Length > 0)
                        {
                            var dialog = dialogs[0];

                            var userField = FindUsernameInput(dialog);
                            var passField = FindPasswordInput(dialog);


                            if (userField != null)
                            {
                                userField.Text = "guylordbiz@gmail.com";
                            }
                            if (passField != null)
                            {
                                passField.Enter("8512James");
                            }

                            var emailLogin = FindEmailLogin(dialog);
                            emailLogin.Click();
                        }
                    }
                }


            }

        }
        static Process GetHoneygainProcess()
        {
            var proc = Process.GetProcesses().Where(pr =>
            {
                try
                {
                    return pr.MainModule.FileName.ToLower().EndsWith("honeygain.exe");
                }
                catch { }
                return false;
            }).FirstOrDefault();

            return proc;
        }
        static FlaUI.Core.Application GetApplication()
        {
            var exe = @"C:\Users\James\Desktop\Honeygain\Honeygain.exe";
            //    var exists = File.Exists(exe);
            //    Assembly asm = Assembly.LoadFrom(exe);
            //    MethodInfo method = asm.EntryPoint;
            //    method.Invoke(null, null);


            var proc = GetHoneygainProcess();
            if (proc != null)
            {
                return FlaUI.Core.Application.Attach(proc.Id); ;
            }
            else
            {
                return FlaUI.Core.Application.Launch(exe);
            }
        }
        static bool IsLoggedIn(Window window)

        {
            var result = false;
            AutomationElement box = null;
            var count = 0;
            do
            {
                //box = window.FindFirstDescendant(cf => cf.ByAutomationId("SettingsProfileControlRoot"))?.FindFirstDescendant(cf => cf.ByAutomationId("AvatarElementRoot"))?.FindAllDescendants(el=>el.ByClassName("TextBlock") ).Where(bx=>bx.AsLabel().Text.Contains("@"))?.FirstOrDefault();
                var boxes = window.FindFirstDescendant(cf => cf.ByAutomationId("SettingsProfileControlRoot"))?.FindAllChildren(el => el.ByClassName("TextBlock"));
                if (boxes == null)
                {
                    if (FindAgreeToTermsButton(window) == null && FindLoginButton(window) == null)
                    {
                        return true;
                    }
                }
                else
                {
                    if (boxes.Length > 0)
                    {
                        box = boxes[boxes.Length - 1];
                    }
                }
                count++;
                Thread.Sleep(1000);
            } while (box == null && count < 5);
            if (box != null)
            {
                var txt = box.AsLabel().Text?.Trim();
                result = txt != null && txt.Contains("@");
            }
            else
            {
                result = box != null;
            }
            return result;

        }
        static Button FindAgreeToTermsButton(Window window)
        {
            Button button = null;
            button = window.FindFirstDescendant(cf => cf.ByText("Agree to Terms of Use"))?.AsButton();

            return button;
        }
        static Button FindLoginButton(Window window)
        {
            Button button = null;
            button = window.FindFirstDescendant(cf => cf.ByText("Log in"))?.AsButton();

            return button;
        }
        static TextBox FindUsernameInput(Window window)
        {
            TextBox box = null;
            box = window.FindFirstDescendant(cf => cf.ByAutomationId("Input"))?.AsTextBox();

            return box;
        }
        static bool IsWindowLoading(Window window)
        {

            var control = window.FindFirstDescendant(cf => cf.ByText("Loading..."));
            bool loading = !(control == null || control.BoundingRectangle.Width == 0);

            return loading;
        }
        static TextBox FindPasswordInput(Window window)
        {
            TextBox box = null;
            box = window.FindFirstDescendant(cf => cf.ByAutomationId("PasswordInput"))?.FindFirstDescendant(el => el.ByAutomationId("PasswordInput"))?.AsTextBox();

            return box;
        }
        static Button FindEmailLogin(Window window)
        {
            Button button = null;
            button = window.FindFirstDescendant(cf => cf.ByAutomationId("PrimaryButtonRoot"))?.FindFirstDescendant(el => el.ByControlType(FlaUI.Core.Definitions.ControlType.Button))?.AsButton();

            return button;
        }
    }
}
