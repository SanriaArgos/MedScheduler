import Link from "next/link";
import { SearchBar } from "./SearchBar";

export const Footer = () => {
    return (
        <footer className="w-full flex flex-col bg-neutral-900 text-neutral-200 py-6 xl:py-12">
            <div className="w-full max-w-screen-2xl mx-auto flex flex-col md:flex-row items-start md:items-center justify-between px-4 xl:px-8 gap-y-6 md:gap-y-0">
                <div className="flex flex-col gap-y-3 xl:gap-y-5 font-medium text-[16px] xl:text-[18px] leading-none">
                    <Link href={"/"} className="hover:text-main transition-colors">Главная</Link>
                    <Link href={"/search"} className="hover:text-main transition-colors">Поиск врачей</Link>
                    <Link href={"/#about"} className="hover:text-main transition-colors">О нас</Link>
                    <Link href={"/site-map"} className="hover:text-main transition-colors">Карта сайта</Link>
                    <Link href={"/api-documentation"} className="hover:text-main transition-colors">Документация API</Link>
                </div>
                <div className="flex flex-col gap-y-3 xl:gap-y-5 font-medium text-[16px] xl:text-[18px] leading-none">
                    <Link href={"/login"} className="hover:text-main transition-colors">Войти</Link>
                    <Link href={"/registration"} className="hover:text-main transition-colors">Регистрация</Link>
                </div>
                <div className="flex flex-col gap-y-3">
                    <p className="text-sm text-neutral-400">Быстрый поиск:</p>
                    <SearchBar placeholder="Врачи, клиники..." size="small" className="max-w-xs" />
                </div>                <div className="text-sm text-neutral-400">
                    <p>Свяжитесь с нами:</p>
                    <p>support@medscheduler.com</p>
                    <p>+7 (000) 000-00-00</p>
                    
                    <div className="mt-4">
                        <a 
                            href="https://github.com/SanriaArgos/MedScheduler" 
                            target="_blank" 
                            rel="noopener noreferrer"
                            className="inline-flex items-center hover:text-main transition-colors"
                        >
                            <svg className="w-5 h-5 mr-2" fill="currentColor" viewBox="0 0 20 20">
                                <path fillRule="evenodd" d="M10 0C4.477 0 0 4.484 0 10.017c0 4.425 2.865 8.18 6.839 9.504.5.092.682-.217.682-.483 0-.237-.008-.868-.013-1.703-2.782.605-3.369-1.343-3.369-1.343-.454-1.158-1.11-1.466-1.11-1.466-.908-.62.069-.608.069-.608 1.003.07 1.531 1.032 1.531 1.032.892 1.53 2.341 1.088 2.91.832.092-.647.35-1.088.636-1.338-2.22-.253-4.555-1.113-4.555-4.951 0-1.093.39-1.988 1.029-2.688-.103-.253-.446-1.272.098-2.65 0 0 .84-.27 2.75 1.026A9.564 9.564 0 0110 4.844c.85.004 1.705.115 2.504.337 1.909-1.296 2.747-1.027 2.747-1.027.546 1.379.203 2.398.1 2.651.64.7 1.028 1.595 1.028 2.688 0 3.848-2.339 4.695-4.566 4.942.359.31.678.921.678 1.856 0 1.338-.012 2.419-.012 2.747 0 .268.18.58.688.482A10.019 10.019 0 0020 10.017C20 4.484 15.522 0 10 0z" clipRule="evenodd" />
                            </svg>
                            GitHub
                        </a>
                    </div>
                </div>
            </div>

            <div className="w-full max-w-screen-2xl mx-auto flex font-medium text-[12px] xl:text-[16px] text-neutral-500 items-center justify-between px-4 xl:px-8 mt-6 xl:mt-12 pt-6 border-t border-neutral-700">
                <span>© 2025, MedScheduler. Все права защищены.</span>
                {/* Можно добавить ссылки на соцсети или др. информацию */}
            </div>
        </footer>
    );
};

