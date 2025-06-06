"use client";
import Image from "next/image";
import { MainSearch } from "@/app/components/MainSearch";
import { AppPromoter } from "@/app/components/AppPromoter";
import Link from "next/link";

export default function Home() {
    return (
        <>
            {/* Hero Section */}
            <section className="text-center py-12 md:py-20">
                <h1 className="text-4xl sm:text-5xl md:text-6xl font-bold text-main2">
                    MedScheduler
                </h1>
                <p className="text-lg sm:text-xl md:text-2xl text-gray-700 mt-4 max-w-2xl mx-auto">
                    Удобная запись к врачам онлайн. Найдите своего специалиста и запишитесь на прием в несколько кликов.
                </p>
                <div className="mt-8">
                    <MainSearch />
                </div>
                <p className="mt-4 text-sm text-gray-500">
                    Начните поиск по ФИО врача, названию клиники или специальности.
                </p>
            </section>

            {/* About Us Section Placeholder */}
            <section id="about" className="py-12 md:py-20 bg-gray-50 rounded-lg">
                <div className="max-w-4xl mx-auto px-4">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-8">О нас</h2>
                    <div className="prose prose-lg max-w-none text-gray-700">
                        <p>
                            MedScheduler — это современная платформа, созданная для упрощения процесса записи на прием к врачу.
                            Мы стремимся сделать здравоохранение более доступным и удобным как для пациентов, так и для медицинских учреждений.
                        </p>
                        <p>
                            Наша миссия — предоставить интуитивно понятный и эффективный инструмент, который экономит ваше время и помогает заботиться о самом главном — вашем здоровье.
                            Мы предлагаем широкий выбор специалистов, удобный поиск по различным критериям и безопасное управление вашими медицинскими записями.
                        </p>
                        <p>
                            Присоединяйтесь к тысячам пользователей, которые уже оценили преимущества MedScheduler!
                        </p>
                    </div>
                </div>
            </section>

            {/* How it works Section */}
            <section className="py-12 md:py-20">
                <div className="max-w-5xl mx-auto px-4">
                    <h2 className="text-3xl font-bold text-center text-main2 mb-12">Как это работает?</h2>
                    <div className="grid md:grid-cols-3 gap-8 text-center">
                        <div className="p-6 shadow-lg rounded-lg bg-white">
                            <div className="text-main text-4xl mb-4">1</div>
                            <h3 className="text-xl font-semibold text-main2 mb-2">Найдите врача</h3>
                            <p className="text-gray-600">Используйте поиск по специальности, ФИО или клинике.</p>
                        </div>
                        <div className="p-6 shadow-lg rounded-lg bg-white">
                            <div className="text-main text-4xl mb-4">2</div>
                            <h3 className="text-xl font-semibold text-main2 mb-2">Выберите время</h3>
                            <p className="text-gray-600">Просмотрите доступные слоты в расписании врача.</p>
                        </div>
                        <div className="p-6 shadow-lg rounded-lg bg-white">
                            <div className="text-main text-4xl mb-4">3</div>
                            <h3 className="text-xl font-semibold text-main2 mb-2">Запишитесь на прием</h3>
                            <p className="text-gray-600">Подтвердите запись и получите уведомление.</p>
                        </div>
                    </div>
                </div>
            </section>

            <AppPromoter />
        </>
    );
}
